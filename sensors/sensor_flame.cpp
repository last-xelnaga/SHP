
#include "sensor_flame.hpp"
#include "wiringPi.h"
#include <stdio.h>


void* flame_working_thread(
        void* p_arg)
{
    sensor_event_class* p_sensor_event = (sensor_event_class*) p_arg;
    unsigned char sensor_gpio_num = p_sensor_event->get_gpio_num ();

    printf ("waiting for event ...\n");
    while (digitalRead (sensor_gpio_num) == HIGH)
        delay (100);
    printf ("alarm on\n") ;
    p_sensor_event->event_on ();

    while (digitalRead (sensor_gpio_num) == LOW)
        delay (100);
    printf ("alarm off\n") ;
    p_sensor_event->event_off ();

    return NULL;
}

sensor_flame_class::sensor_flame_class(
        unsigned char gpio_num,
        const char* p_name) :
        sensor_event_class (gpio_num, p_name, "FLAME")
{

}

void sensor_flame_class::activate (
        void)
{
    pinMode (sensor_gpio_num, INPUT);

    // Create worker thread
    pthread_create (&pth, NULL, flame_working_thread, this);
    printf ("%s button working thread create\n", p_sensor_name);
}
