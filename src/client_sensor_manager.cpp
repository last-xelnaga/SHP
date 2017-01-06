
#include "client_sensor_manager.hpp"
#include "debug.hpp"
#include "message.hpp"
#include "client_config.hpp"
#include "queue.hpp"

// triggers
#include "sensors/sensor_event.hpp"
#include "sensors/sensor_dht.hpp"

// slaves
#include "sensors/sensor_led.hpp"
#include "sensors/sensor_relay.hpp"
#include "sensors/sensor_servo.hpp"

#include <wiringPi.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define EVENT_ON        "1"
#define EVENT_OFF       "0"

static pthread_mutex_t sensor_mutex = PTHREAD_MUTEX_INITIALIZER;


static void event_on(
        sensor_manager_class* const p_sensor_manager,
        sensor_event_class* const p_sensor)
{
    printf("event_on callback\n");

    message_class* p_message = new message_class(message_class::send_event);
    p_message->add_time_to_message();
    p_message->add_string_to_message(message_class::sensor_type, p_sensor->get_type());
    p_message->add_string_to_message(message_class::sensor_name, p_sensor->get_name());
    p_message->add_num_to_message(message_class::sensor_gpio, p_sensor->get_gpio_num());
    p_message->add_string_to_message(message_class::sensor_data, EVENT_ON);

    p_sensor_manager->add_to_queue(p_message);
}

static void event_off(
        sensor_manager_class* const p_sensor_manager,
        sensor_event_class* const p_sensor)
{
    printf("event_off callback\n");

    message_class* p_message = new message_class(message_class::send_event);
    p_message->add_time_to_message();
    p_message->add_string_to_message(message_class::sensor_type, p_sensor->get_type());
    p_message->add_string_to_message(message_class::sensor_name, p_sensor->get_name());
    p_message->add_num_to_message(message_class::sensor_gpio, p_sensor->get_gpio_num());
    p_message->add_string_to_message(message_class::sensor_data, EVENT_OFF);

    p_sensor_manager->add_to_queue(p_message);

    p_sensor->activate();
}

/*static void led_on (
        )
{

}

static void led_off (
        )
{

}*/

sensor_manager_class::sensor_manager_class(
        void)
{
    is_initialized = false;
    p_queue = NULL;
}

error_code_t sensor_manager_class::setup_board(
        void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        if (is_initialized == true)
        {
            // early return :(
            DEBUG_LOG_TRACE_END(result)
            return result;
        }
    }

    if (result == RESULT_OK)
    {
        if (geteuid() != 0)
        {
            DEBUG_LOG_MESSAGE("need to be root to run. exit");
            result = RESULT_INVALID_STATE;
        }
    }

    if (result == RESULT_OK)
    {
        int res = wiringPiSetup();
        if (res == -1)
        {
            DEBUG_LOG_MESSAGE("board setup has failed. exit");
            result = RESULT_SYSTEM_ERROR;
        }
    }

    if (result == RESULT_OK)
    {
        is_initialized = true;
    }

    DEBUG_LOG_TRACE_END(result)
    return result;
}

error_code_t sensor_manager_class::setup_sensors(
        queue_class* const p_queue_)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    p_queue = p_queue_;

    if (result == RESULT_OK)
    {
        //result = setup_board();
    }

    if (result == RESULT_OK)
    {
        // enter the critical section, lock the mutex
        pthread_mutex_lock(&sensor_mutex);

        for (unsigned int i = 0; i < get_sensor_settings()->sensors_count && result == RESULT_OK; i ++)
        {
            sensor_settings_t* p_sensor = &get_sensor_settings()->p_sensors[i];
            result = add_sensor(p_sensor->p_name, p_sensor->gpio, p_sensor->p_type);
        }

        // we finished with critical sectio, so unlock the mutex
        pthread_mutex_unlock(&sensor_mutex);
    }

    DEBUG_LOG_TRACE_END(result)
    return result;
}

error_code_t sensor_manager_class::add_sensor (
        const char* p_name,
        const unsigned int gpio,
        const char* p_type)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        if (strcmp (p_type, "BUTTON") == 0)
        {
            sensor_button_class* p_button = new sensor_button_class (gpio, p_name);
            p_button->set_event_callback (event_on, event_off, this);
            sensors.push_back (p_button);
        }
        else if (strcmp (p_type, "DHT11") == 0)
        {
            sensor_dht11_class* p_dth11 = new sensor_dht11_class (gpio, p_name);
            p_dth11->set_event_callback (event_on, event_off, this);
            sensors.push_back (p_dth11);
        }
        else if (strcmp (p_type, "FLAME") == 0)
        {
            sensor_flame_class* p_flame = new sensor_flame_class (gpio, p_name);
            p_flame->set_event_callback (event_on, event_off, this);
            sensors.push_back (p_flame);
        }
        else if (strcmp (p_type, "PIR") == 0)
        {
            sensor_pir_class* p_pir = new sensor_pir_class (gpio, p_name);
            p_pir->set_event_callback (event_on, event_off, this);
            sensors.push_back (p_pir);
        }
        else if (strcmp (p_type, "LED") == 0)
        {
            sensor_led_class* p_led = new sensor_led_class (gpio, p_name);
            sensors.push_back (p_led);
        }
        else
        {
            DEBUG_LOG_MESSAGE ("unknown sensor type. skip\n");
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t sensor_manager_class::activate_sensors (
        void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // enter the critical section, lock the mutex
    pthread_mutex_lock (&sensor_mutex);

    for (unsigned int i = 0; i < sensors.size(); i ++)
    {
        sensors[i]->activate ();
    }

    // we finished with critical sectio, so unlock the mutex
    pthread_mutex_unlock (&sensor_mutex);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

void sensor_manager_class::add_to_queue (
        message_class* const p_message)
{
    DEBUG_LOG_TRACE_BEGIN

    if (p_queue != NULL)
    {
        p_queue->add (p_message);
    }

    DEBUG_LOG_TRACE_END (RESULT_OK)
}

sensor_manager_class::~sensor_manager_class (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    // enter the critical section, lock the mutex
    pthread_mutex_lock (&sensor_mutex);

    for (unsigned int i = 0; i < sensors.size(); i ++)
    {
        delete sensors[i];
    }

    sensors.clear ();

    // we finished with critical sectio, so unlock the mutex
    pthread_mutex_unlock (&sensor_mutex);

    DEBUG_LOG_TRACE_END (RESULT_OK)
}
