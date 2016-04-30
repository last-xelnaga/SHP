
#include <string.h>
#include <stdio.h>

#include "sensor_servo.hpp"
#include "wiringPi.h"

sensor_servo_class::sensor_servo_class (
    unsigned char gpio_num,
    const char* p_name) : sensor_class(gpio_num, p_name)
{
    sensor_gpio_num = gpio_num;
    strcpy(p_sensor_name, p_name);


}

sensor_servo_class::~sensor_servo_class (
    void)
{

}

void sensor_servo_class::set_angle (
    unsigned char angle)
{
    // convert angle to microseconds
    long wait_time = ((long)angle * 5.56) + 1000;

    // generate a pulse
    digitalWrite(sensor_gpio_num, HIGH);
    delayMicroseconds((unsigned)wait_time);
    digitalWrite(sensor_gpio_num, LOW);

    // wait for an action
    delay(20);
}

void sensor_servo_class::activate (
    void)
{
    pinMode(sensor_gpio_num, OUTPUT);
}
