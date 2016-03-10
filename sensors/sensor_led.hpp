
#ifndef SENSOR_LED_HPP
#define SENSOR_LED_HPP

#include "sensor.hpp"

class sensor_led_class : public sensor_class
{
    int active;

    /*void flashing (
        unsigned int ms)
    {
        while(active)
        {
            switch_on();
            delay(ms);
            switch_off();
        }
    }*/

public:
    sensor_led_class(unsigned char gpio_num, const char* p_name);
    /*{
        sensor_gpio_num = gpio_num;
        strcpy(p_sensor_name, p_name);


    }*/

    virtual ~sensor_led_class(void);
    /*{

    }*/

    void switch_on(
            void);
    /*{
        digitalWrite(sensor_gpio_num, 1);
    }*/

    void switch_off(
            void);
    /*{
        digitalWrite(sensor_gpio_num, 0);
    }*/

    void flash_on(
        unsigned int ms);
    /*{
        active = 1;
        // start thread 
    }*/

    void flash_off(
        void);
    /*{
        active = 0;
    }*/

    virtual void activate(
        void);
    /*{
        pinMode(sensor_gpio_num, OUTPUT);
        switch_off();
    }*/
};

#endif // SENSOR_LED_HPP
 
