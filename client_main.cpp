
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "config.hpp"
#include "sensor_manager.hpp"
#include "queue_manager.hpp"
#include "debug.hpp"
#include "socket.hpp"

int led_works = 0;
int led_works2 = 0;

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

int main (
    int argc,
    char **argv)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN


    if (result == RESULT_OK)
    {
        result = config ()->read_config ("shp_client.cfg");
    }

    sensor_manager_class* p_sensor_manager = new sensor_manager_class ();

    if (result == RESULT_OK)
    {
        /*std::vector <sensor_settings_t>::const_iterator sensor;
        for (sensor = config ()->sensors.begin(); sensor != config ()->sensors.end() && result == RESULT_OK; ++ sensor)
        {
            result = p_sensor_manager->add_sensor (sensor->p_name, sensor->gpio, sensor->p_type);
        }*/
        p_sensor_manager->add_sensors ();
    }

    if (result == RESULT_OK)
    {
        client_socket_class client_socket (config ()->network.address.c_str (),
                config ()->network.port);

        queue_manager_class::instance ()->run (&client_socket);
    }

    delete p_sensor_manager;

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
