
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "config.hpp"
#include "sensors/sensor_button.hpp"
#include "sensors/sensor_led.hpp"

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
    LED* p_status_led = (LED*) p_user_data;
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

    setup();

    LED* p_status_led = new LED(0, "status");
    p_status_led->activate();

    Button* p_reset_button = new Button(8, "reset");
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
