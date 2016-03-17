
#include <string.h>
#include <stdio.h>

#include "sensor_relay.hpp"
#include "../external/wiringPi/wiringPi.h"

sensor_relay_class::sensor_relay_class (
    unsigned char gpio_num,
    const char* p_name)
{
    sensor_gpio_num = gpio_num;
    strcpy(p_sensor_name, p_name);


}

sensor_relay_class::~sensor_relay_class (
    void)
{

}

void sensor_relay_class::switch_on (
        void)
{
    digitalWrite(sensor_gpio_num, 1);
}

void sensor_relay_class::switch_off (
        void)
{
    digitalWrite(sensor_gpio_num, 0);
}

void sensor_relay_class::activate (
    void)
{
    pinMode(sensor_gpio_num, OUTPUT);
    switch_off();
}
