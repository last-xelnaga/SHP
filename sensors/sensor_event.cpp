
#include "sensor_event.hpp"
#include "wiringPi.h"
#include <unistd.h> // sleep
#include <stdio.h>
#include <pthread.h>


void* working_thread (
        void* p_arg)
{
    sensor_event_class* p_sensor_event = (sensor_event_class*) p_arg;

    while (1)
    {
        //printf ("waiting for event ...\n");
        if (p_sensor_event->is_active ())
        {
            p_sensor_event->do_task ();
        }

        usleep (100);
    }

    return NULL;
}

sensor_event_class::sensor_event_class (
        unsigned char sensor_gpio_num_,
        const char * p_sensor_name_,
        const char * p_sensor_type_) :
        sensor_class (sensor_gpio_num_, p_sensor_name_, p_sensor_type_)
{
    pthread_create (&pth, NULL, working_thread, this);
}

 void sensor_event_class::set_event_callback (
            f_event_callback p_event_on_callback_,
            f_event_callback p_event_off_callback_,
            const sensor_manager_class* p_sensor_manager_)
{
    p_event_on_callback = p_event_on_callback_;
    p_event_off_callback = p_event_off_callback_;
    p_sensor_manager = (sensor_manager_class*) p_sensor_manager_;
}

void sensor_event_class::event_on (
        void)
{
    if (p_event_on_callback)
        p_event_on_callback (p_sensor_manager, this);
}

void sensor_event_class::event_off (
        void)
{
    if (p_event_off_callback)
        p_event_off_callback (p_sensor_manager, this);
}

sensor_event_class::~sensor_event_class (
        void)
{
    pthread_cancel (pth);
    //printf ("wait for join\n");
    // wait for our thread to finish before continuing
    pthread_join (pth, NULL);
}

void sensor_event_class::activate (
        void)
{
    if (activated)
        return;

    sensor_setup ();

    activated = true;
    //printf ("pir working thread created\n");
}

void sensor_event_class::sensor_setup (
        void)
{
    pinMode (sensor_gpio_num, INPUT);
}

void sensor_event_class::do_task (
        void)
{
    activated = false;

    printf ("waiting for event ...\n");
    while (digitalRead (sensor_gpio_num) == HIGH)
        delay (100);
    printf ("button pressed\n") ;
    event_on ();

    while (digitalRead (sensor_gpio_num) == LOW)
        delay (100);
    printf ("button released\n") ;
    event_off ();
}

sensor_button_class::sensor_button_class (
        unsigned char sensor_gpio_num_,
        const char * p_sensor_name_) :
        sensor_event_class (sensor_gpio_num_, p_sensor_name_, "BUTTON")
{

}

sensor_flame_class::sensor_flame_class(
        unsigned char gpio_num,
        const char* p_name) :
        sensor_event_class (gpio_num, p_name, "FLAME")
{

}

sensor_pir_class::sensor_pir_class (
        unsigned char gpio_num,
        const char* p_name) :
        sensor_event_class (gpio_num, p_name, "PIR")
{

}

void sensor_pir_class::do_task (
        void)
{
    activated = false;
    printf ("waiting for event ...\n");

    sleep (3);
    printf ("motion detected\n") ;
    event_on ();

    sleep (1);
    printf ("motion off\n") ;
    event_off ();
}

sensor_reed_class::sensor_reed_class (
        unsigned char gpio_num,
        const char* p_name) :
        sensor_event_class (gpio_num, p_name, "REED")
{

}
