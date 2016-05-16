
#include "sensor_button.hpp"
#include "wiringPi.h"
#include <stdio.h>


void* button_working_thread (
        void* p_arg)
{
    sensor_event_class* p_sensor_event = (sensor_event_class*) p_arg;
    unsigned char sensor_gpio_num = p_sensor_event->get_gpio_num ();

    printf ("waiting for event ...\n");
    while (digitalRead (sensor_gpio_num) == HIGH)
        delay (100);
    printf ("button pressed\n") ;
    p_sensor_event->event_on ();

    while (digitalRead (sensor_gpio_num) == LOW)
        delay (100);
    printf ("button released\n") ;
    p_sensor_event->event_off ();

    return NULL;
}

sensor_button_class::sensor_button_class (
        unsigned char sensor_gpio_num_,
        const char * p_sensor_name_) :
        sensor_event_class (sensor_gpio_num_, p_sensor_name_, "BUTTON")
{

}

void sensor_button_class::activate (
        void)
{
    pinMode (sensor_gpio_num, INPUT);

    // create worker thread
    pthread_create (&pth, NULL, button_working_thread, this);
}
