
#include <string.h>
#include <stdio.h>

#include "sensor_led.hpp"
#include "wiringPi.h"

sensor_led_class::sensor_led_class(unsigned char gpio_num, const char* p_name) : sensor_class(gpio_num, p_name, "LED")
{
    sensor_gpio_num = gpio_num;
    strcpy(p_sensor_name, p_name);


}

sensor_led_class::~sensor_led_class(void)
{

}

void sensor_led_class::switch_on(
        void)
{
    digitalWrite(sensor_gpio_num, 1);
}

void sensor_led_class::switch_off(
        void)
{
    digitalWrite(sensor_gpio_num, 0);
}

void sensor_led_class::flash_on(
    unsigned int ms)
{
    active = 1;
    // start thread
}

void sensor_led_class::flash_off(
    void)
{
    active = 0;
}

void sensor_led_class::activate(
    void)
{
    pinMode(sensor_gpio_num, OUTPUT);
    switch_off();
}
