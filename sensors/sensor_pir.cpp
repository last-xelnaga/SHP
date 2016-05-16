
#include "sensor_pir.hpp"
#include "wiringPi.h"
#include <stdio.h>
#include <unistd.h> // sleep


void* pir_working_thread (
        void* p_arg)
{
    sensor_event_class* p_sensor_event = (sensor_event_class*) p_arg;
    //unsigned char sensor_gpio_num = p_pir->get_gpio_num ();

    printf ("waiting for event ...\n");
    /*while (digitalRead (sensor_gpio_num) == LOW)
        delay (100);*/
    sleep (5);
    printf ("motion detected\n") ;
    p_sensor_event->event_on ();

    /*while (digitalRead (sensor_gpio_num) == HIGH)
        delay (100);*/
    sleep (1);
    printf ("motion off\n") ;
    p_sensor_event->event_off ();

    return NULL;
}

sensor_pir_class::sensor_pir_class (
        unsigned char gpio_num,
        const char* p_name) : sensor_event_class (gpio_num, p_name, "PIR")
{

}

void sensor_pir_class::activate (
        void)
{
    pinMode (sensor_gpio_num, INPUT);

    // Create worker thread
    pthread_create (&pth, NULL, pir_working_thread, this);
    printf ("pir working thread create\n");
}
