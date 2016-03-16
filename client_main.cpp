
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "config.hpp"
#include "sensors/sensor_manager.hpp"
#include "sensors/sensor_button.hpp"
#include "sensors/sensor_led.hpp"
#include "sensors/sensor_dht11.hpp"

int led_works = 0;

int reset_callback(
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

int main (
    int argc,
    char **argv)
{
    // ignore SIGPIPE signal
    /*if (SIG_ERR == signal(SIGPIPE, SIG_IGN))
    {
        return -1;
    }*/

    Config::instance()->read_config("shp_client.cfg");

    sensor_manager_class::instance()->setup();

    sensor_led_class* p_status_led = new sensor_led_class(0, "status");
    p_status_led->activate();

    sensor_button_class* p_reset_button = new sensor_button_class(8, "reset");
    p_reset_button->set_callback(reset_callback, p_status_led);
    p_reset_button->activate();

    int i = 10;
    while (i --)
    {
        reset_callback(p_status_led);
        sleep(1);
    }

    delete p_reset_button;
    delete p_status_led;

    return 0;
}
