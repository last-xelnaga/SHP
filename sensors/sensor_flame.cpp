
#include <string.h>
#include <stdio.h>

#include "sensor_flame.hpp"
#include "wiringPi.h"

void* flame_working_thread(
    void* p_arg)
{
    sensor_flame_class* p_flame = (sensor_flame_class*)p_arg;
    unsigned char sensor_gpio_num = p_flame->get_gpio_num();
    void* p_user_data = NULL;
    button_up_cb p_button_up_cb = p_flame->get_callback(&p_user_data);

    printf ("Waiting for alarm ...\n");
    while (digitalRead(sensor_gpio_num) == HIGH)
        delay(100);
    printf ("alarm\n") ;
    p_button_up_cb(p_user_data);

    while (digitalRead(sensor_gpio_num) == LOW)
        delay(100);
    printf ("button released\n") ;

    p_flame->activate();

    return NULL;
}

sensor_flame_class::sensor_flame_class(
    unsigned char gpio_num,
    const char* p_name) : sensor_class(gpio_num, p_name)
{
    sensor_gpio_num = gpio_num;
    strcpy(p_sensor_name, p_name);
}

sensor_flame_class::~sensor_flame_class(
    void)
{
    pthread_cancel(pth);

    printf("wait for join\n");
    // wait for our thread to finish before continuing
    pthread_join(pth, NULL);

    printf("button destroyed\n");
}

void sensor_flame_class::activate(
        void)
{
    pinMode(sensor_gpio_num, INPUT);

    // Create worker thread
    pthread_create(&pth, NULL, flame_working_thread, this);
    printf("%s button working thread create\n", p_sensor_name);
}
