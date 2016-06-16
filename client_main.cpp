
#include "config.hpp"
#include "sensor_manager.hpp"
#include "debug.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "socket_client.hpp"

#include <stdio.h>
#include <unistd.h>
//#include <signal.h>

//int led_works = 0;
//int led_works2 = 0;

/*int reset_callback (
        const void* p_user_data)
{
    sensor_led_class* p_status_led = (sensor_led_class*) p_user_data;
    printf("reset_callback\n") ;

    if (led_works)
    {
        p_status_led->switch_off();
        led_works = 0;
    }
    else
    {
        p_status_led->switch_on();
        led_works = 1;
    }

    return 0;
}

int relay_callback (
        const void* p_user_data)
{
    printf("relay_callback\n") ;
    sensor_relay_class* p_relay = (sensor_relay_class*) p_user_data;

    p_relay->switch_on();
    sleep(4);
    p_relay->switch_off();

    return 0;
}*/

/*int pir_callback (
        const void* p_user_data)
{
    sensor_led_class* p_status_led = (sensor_led_class*) p_user_data;
    printf("pir_callback\n") ;

    if (led_works2)
    {
        p_status_led->switch_off();
        led_works2 = 0;
    }
    else
    {
        p_status_led->switch_on();
        led_works2 = 1;
    }

    return 0;
}*/

static config_class* config (
        void)
{
   return config_class::instance ();
}

error_code_t send_message (
        client_socket_class* const p_socket,
        message_class* const p_message)
{
    error_code_t result = RESULT_OK;
    message_class* p_answer = NULL;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        result = p_socket->connect (config ()->network.address.c_str (),
                config ()->network.port);
    }

    if (result == RESULT_OK)
    {
        result = p_socket->send_and_receive (p_message, &p_answer);
    }

    if (result == RESULT_OK)
    {
        // check the answer
    }

    p_socket->close ();

    delete p_answer;

    DEBUG_LOG_TRACE_END (result)
    return result;
}

int main (
        int argc,
        char **argv)
{
    error_code_t result = RESULT_OK;
    sensor_manager_class* p_sensor_manager = NULL;
    queue_class* p_queue = NULL;
    client_socket_class* p_socket = NULL;
    DEBUG_LOG_TRACE_BEGIN

    // create and read config
    if (result == RESULT_OK)
    {
        result = config ()->read_config ("shp_client.cfg");
    }

    // organize a queue
    if (result == RESULT_OK)
    {
        p_queue = new queue_class ();
    }

    // create sensor manager, initialize and verify settings
    if (result == RESULT_OK)
    {
        p_sensor_manager = new sensor_manager_class ();
        p_sensor_manager->setup_sensors (p_queue);
    }

    // send config
    if (result == RESULT_OK)
    {
        p_socket = new client_socket_class ();
    }

    if (result == RESULT_OK)
    {
        p_sensor_manager->activate_sensors ();
    }

    // loop
    if (result == RESULT_OK)
    {
        //int x = 100;

        while (result == RESULT_OK /*&& x >= 0*/)
        {
            unsigned int message_id = 0;
            message_class* p_message = NULL;

            result = p_queue->peek (&message_id, &p_message);
            if (message_id == 0)
            {
                usleep (500000);
                continue;
            }

            if (result == RESULT_OK)
            {
                result = send_message (p_socket, p_message);
            }

            if (result == RESULT_OK)
            {
                p_queue->remove (message_id);
                //x --;
            }
        }
    }

    delete config_class::instance ();
    delete p_sensor_manager;
    delete p_queue;
    delete p_socket;

    /*sensor_led_class* p_status_led = new sensor_led_class(0, "status");
    p_status_led->activate();

    sensor_led_class* p_status_led_pir = new sensor_led_class(22, "status");
    p_status_led_pir->activate();

    sensor_pir_class* p_pir = new sensor_pir_class(3, "pir");
    p_pir->set_callback(pir_callback, p_status_led_pir);
    p_pir->activate();

    sensor_button_class* p_reset_button = new sensor_button_class(8, "reset");
    p_reset_button->set_trigger_cb(reset_callback, p_status_led);
    p_reset_button->activate();

    sensor_dht11_class* p_dth11 = new sensor_dht11_class(9, "temp1");
    p_dth11->activate();*/

    //sensor_dht11_class* p_dth112 = new sensor_dht11_class(3, "temp2");
    //p_dth112->activate();

    /*sensor_relay_class* p_relay = new sensor_relay_class(2, "relay");
    p_relay->activate();

    sensor_relay_class* p_relay2 = new sensor_relay_class(1, "relay");
    p_relay2->activate();

    sensor_button_class* p_relay_button = new sensor_button_class(7, "p_relay_button");
    p_relay_button->set_trigger_cb(relay_callback, p_relay);
    p_relay_button->activate();

    sensor_button_class* p_relay_button2 = new sensor_button_class(21, "p_relay_button2");
    p_relay_button2->set_trigger_cb(relay_callback, p_relay2);
    p_relay_button2->activate();*/

    DEBUG_LOG_TRACE_END (result)
    return result;
}
