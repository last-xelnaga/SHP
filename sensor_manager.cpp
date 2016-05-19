
#include "wiringPi.h"
#include "sensor_manager.hpp"
#include "debug.hpp"
#include "queue_manager.hpp"
#include "message.hpp"

// triggers
#include "sensors/sensor_button.hpp"
#include "sensors/sensor_dht11.hpp"
#include "sensors/sensor_flame.hpp"
#include "sensors/sensor_pir.hpp"

// slaves
#include "sensors/sensor_led.hpp"
#include "sensors/sensor_relay.hpp"
#include "sensors/sensor_servo.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define EVENT_ON        "1"
#define EVENT_OFF       "0"


void event_on (
        const sensor_manager_class* p_sensor_manager,
        const sensor_event_class* p_sensor)
{
    printf ("event_on callback\n") ;

    sensor_class* p_sensor_temp = (sensor_class*) p_sensor;

    message_class* p_message = new message_class (message_class::send_event);
    p_message->add_time_to_message ();
    p_message->add_string_to_message (message_class::sensor_type, p_sensor_temp->get_type ());
    p_message->add_string_to_message (message_class::sensor_name, p_sensor_temp->get_name ());
    p_message->add_string_to_message (message_class::sensor_data, EVENT_ON);

    queue_manager_class::instance ()->add_message (p_message);
}

void event_off (
        const sensor_manager_class* p_sensor_manager,
        const sensor_event_class* p_sensor)
{
    printf ("event_off callback\n") ;

    sensor_class* p_sensor_temp = (sensor_class*) p_sensor;

    message_class* p_message = new message_class (message_class::send_event);
    p_message->add_time_to_message ();
    p_message->add_string_to_message (message_class::sensor_type, p_sensor_temp->get_type ());
    p_message->add_string_to_message (message_class::sensor_name, p_sensor_temp->get_name ());
    p_message->add_string_to_message (message_class::sensor_data, EVENT_OFF);

    queue_manager_class::instance ()->add_message (p_message);

    p_sensor_temp->activate ();
}


sensor_manager_class *sensor_manager_class::p_instance = 0;

sensor_manager_class::sensor_manager_class (
        void)
{
    is_initialized = false;
}

error_code_t sensor_manager_class::setup_board (
        void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        if (is_initialized == true)
        {
            // early return :(
            DEBUG_LOG_TRACE_END (result)
            return result;
        }
    }

    if (result == RESULT_OK)
    {
        if (geteuid () != 0)
        {
            DEBUG_LOG_MESSAGE ("need to be root to run. exit");
            result = RESULT_INVALID_STATE;
        }
    }

    if (result == RESULT_OK)
    {
        int res = wiringPiSetup ();
        if (res == -1)
        {
            DEBUG_LOG_MESSAGE ("board setup has failed. exit");
            result = RESULT_SYSTEM_ERROR;
        }
    }

    if (result == RESULT_OK)
    {
        is_initialized = true;
    }

    DEBUG_LOG_TRACE_END (result)
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
        //result = setup_board ();
    }

    if (result == RESULT_OK)
    {
        if (strcmp (p_type, "BUTTON") == 0)
        {
            sensor_button_class* p_button = new sensor_button_class (gpio, p_name);
            p_button->set_event_callback (event_on, event_off, this);
            p_button->activate ();
        }
        else if (strcmp (p_type, "DHT11") == 0)
        {
            sensor_dht11_class* p_dth11 = new sensor_dht11_class (gpio, p_name);
            p_dth11->activate ();
        }
        else if (strcmp (p_type, "FLAME") == 0)
        {
            sensor_flame_class* p_flame = new sensor_flame_class (gpio, p_name);
            p_flame->set_event_callback (event_on, event_off, this);
            p_flame->activate ();
        }
        else if (strcmp (p_type, "PIR") == 0)
        {
            sensor_pir_class* p_pir = new sensor_pir_class (gpio, p_name);
            p_pir->set_event_callback (event_on, event_off, this);
            p_pir->activate ();
        }
        else if (strcmp (p_type, "LED") == 0)
        {
            sensor_led_class* p_led = new sensor_led_class (gpio, p_name);
            p_led->activate ();
        }
        else
        {
            DEBUG_LOG_MESSAGE ("unknown sensor type. skip\n");
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}
