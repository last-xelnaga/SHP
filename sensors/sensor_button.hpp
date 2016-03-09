
#ifndef SENSOR_BUTTON_HPP
#define SENSOR_BUTTON_HPP

#include "sensor.hpp"
#include <string.h>
#include <pthread.h>
#include <stdio.h>


typedef int (*wait_for_button_cb)(
        const void* p_user_data);

void *wait_button(void *arg)
{
    while(1)
    {
        printf("Waiting for button ... ");
        while(digitalRead(/*sensor_gpio_num*/8) == HIGH)
            delay(100);
        printf ("Got it\n") ;
    //p_wait_for_button_cb(p_user_data);
    }

    return NULL;
}

class Button : public Sensor
{
    wait_for_button_cb p_wait_for_button_cb = NULL;
    void* p_user_data = NULL;
    pthread_t pth;  // this is our thread identifier



public:
    Button(unsigned char gpio_num, const char* p_name)
    {
        sensor_gpio_num = gpio_num;
        strcpy(p_sensor_name, p_name);
    }

    virtual ~Button(void)
    {

        pthread_cancel(pth);

        printf("wait for join\n");
        /* wait for our thread to finish before continuing */
        pthread_join(pth, NULL /* void ** return value could go here */);

        printf("button destroyed\n");
    }

    void set_callback(
            wait_for_button_cb p_callback,
            const void* p_data)
    {
        p_wait_for_button_cb = p_callback;
        p_user_data = (void*)p_data;

    }

    virtual void activate(
            void)
    {
        pinMode(sensor_gpio_num, INPUT);

        /* Create worker thread */
        pthread_create(&pth, NULL, wait_button, NULL);
        printf("button work thread create\n");
    }
};

#endif // SENSOR_BUTTON_HPP
