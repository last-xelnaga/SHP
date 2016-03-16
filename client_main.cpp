
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "config.hpp"
#include "sensors/sensor_button.hpp"
#include "sensors/sensor_led.hpp"
#include "sensors/sensor_dht11.hpp"

#include "external/wiringPi/wiringPi.h"

void setup (
    void)
{
    if (geteuid() != 0)
    {
        printf("Need to be root to run\n");
        exit(0);
    }

    if (wiringPiSetup() == -1)
        exit(1);

    printf("setup OK\n") ;
}

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

int relay_callback(
        const void* p_user_data)
{
    printf("relay_callback\n") ;

    pinMode(2, OUTPUT);
    digitalWrite(2, 1);
    sleep(1);
    digitalWrite(2, 0);

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

    setup();

    sensor_led_class* p_status_led = new sensor_led_class(0, "status");
    p_status_led->activate();

    sensor_button_class* p_reset_button = new sensor_button_class(8, "reset");
    p_reset_button->set_callback(reset_callback, p_status_led);
    p_reset_button->activate();

    sensor_dht11_class* p_dth11 = new sensor_dht11_class(9, "temp");
    p_dth11->activate();

    sensor_button_class* p_relay_button = new sensor_button_class(7, "reset");
    p_relay_button->set_callback(relay_callback, NULL);
    p_relay_button->activate();

    //int i = 10;
    while (1 /*i --*/)
    {
        //reset_callback(p_status_led);
        p_dth11->show();
        sleep(2);
    }

    delete p_relay_button;
    delete p_reset_button;
    delete p_status_led;
    delete p_dth11;

    return 0;
}
