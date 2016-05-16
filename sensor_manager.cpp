
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

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


class test_message_class : public message_class
{
public:
    char p_text [128];
    unsigned int data;

public:
    test_message_class (
        void) : message_class (message_class::send_temp)
    {
        memset (p_text, 0, sizeof (p_text));
        data = 0;
    }

    void set_new_data (
        const char* p_new_text,
        unsigned int new_data)
    {
        strcpy (p_text, p_new_text);
        data = new_data;
    }

    void pack_payload (
            void)
    {
        p_raw_payload = new unsigned char[128];
        unsigned char* p_pointer = p_raw_payload;

        // add size of the text
        int text_length = strlen (p_text) + 1;
        memcpy (p_pointer, &text_length, sizeof (int));
        p_pointer += sizeof (int);
        memcpy (p_pointer, p_text, text_length);
        p_pointer += text_length;

        // add data
        memcpy (p_pointer, &data, sizeof (int));
        p_pointer += sizeof (int);

        update_header (p_pointer - p_raw_payload);
    }

    void unpack_payload (
            void)
    {
        unsigned char* p_pointer = p_raw_payload;

        // get text
        int text_length = 0;
        memcpy (&text_length, p_pointer, sizeof (int));
        p_pointer += sizeof (int);
        memcpy (p_text, p_pointer, text_length);
        p_pointer += text_length;

        // get data
        memcpy (&data, p_pointer, sizeof (int));
        p_pointer += sizeof (int);
    }
};

void event_on (
        const sensor_manager_class* p_sensor_manager,
        const sensor_event_class* p_sensor)
{
    printf ("event_on callback\n") ;

    sensor_class* p_sensor_temp = (sensor_class*) p_sensor;

    test_message_class* p_message = new test_message_class ();
    p_message->set_new_data (p_sensor_temp->get_name (), 1);
    p_message->pack_payload ();

    queue_manager_class::instance ()->add_message (p_message);
}

void event_off (
        const sensor_manager_class* p_sensor_manager,
        const sensor_event_class* p_sensor)
{
    printf ("event_off callback\n") ;

    sensor_class* p_sensor_temp = (sensor_class*) p_sensor;

    test_message_class* p_message = new test_message_class ();
    p_message->set_new_data (p_sensor_temp->get_name (), 0);
    p_message->pack_payload ();

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
            // early return ((
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
            //p_flame->set_event_callback (event_on, event_off, this);
            //p_flame->activate ();
        }
        else if (strcmp (p_type, "PIR") == 0)
        {
            sensor_pir_class* p_pir = new sensor_pir_class (gpio, p_name);
            p_pir->set_event_callback (event_on, event_off, this);
            p_pir->activate ();
        }
        else if (strcmp (p_type, "LED") == 0)
        {
            sensor_led_class* p_status_led_pir = new sensor_led_class (gpio, p_name);
            p_status_led_pir->activate ();
        }
        else
        {
            DEBUG_LOG_MESSAGE ("unknown sensor type. skip\n");
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}
