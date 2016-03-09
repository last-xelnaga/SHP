
#ifndef SENSOR_BUTTON_HPP
#define SENSOR_BUTTON_HPP

#include "sensor.hpp"
#include <string.h>

typedef int (*wait_for_button_cb)(
        const void* p_user_data);

class Button : public Sensor
{
    wait_for_button_cb p_wait_for_button_cb = NULL;
    void* p_user_data = NULL;

    void wait_button(
        void)
    {
        //printf ("Waiting for button ... ") ; fflush (stdout) ;
        while(digitalRead(sensor_gpio_num) == HIGH)
            delay(100);
        //printf ("Got it\n") ;
        p_wait_for_button_cb(p_user_data);
    }

public:
    Button(unsigned char gpio_num, const char* p_name)
    {
        sensor_gpio_num = gpio_num;
        strcpy(p_sensor_name, p_name);
    }

    virtual ~Button(void)
    {

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
    }
};

#endif // SENSOR_BUTTON_HPP
