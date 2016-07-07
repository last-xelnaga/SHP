
#include "sensor_servo.hpp"
#include "wiringPi.h"


sensor_servo_class::sensor_servo_class (
    unsigned char gpio_num,
    const char* p_name) : sensor_class (gpio_num, p_name, "SERVO")
{
    set_params (500, 2500, 50);
}

void sensor_servo_class::set_params (
        unsigned int min_time,
        unsigned int max_time,
        unsigned int frequence)
{
    wait_time = 1000000 / frequence;

    b = min_time;
    k = (max_time - min_time) / 180;
}

void sensor_servo_class::set_angle (
        unsigned char angle)
{
    // convert angle to microseconds
    double pulse_time = angle * k + b;

    int i = 25;
    do
    {
        // generate a pulse
        digitalWrite (sensor_gpio_num, HIGH);
        delayMicroseconds ((unsigned int) pulse_time);
        digitalWrite (sensor_gpio_num, LOW);

        delayMicroseconds ((unsigned int) wait_time - pulse_time);
        i --;
    } while (i > 0);
}

void sensor_servo_class::activate (
    void)
{
    digitalWrite (sensor_gpio_num, LOW);
    pinMode (sensor_gpio_num, OUTPUT);
}

sensor_servo_class::~sensor_servo_class (
    void)
{

}
