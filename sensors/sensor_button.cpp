
#include <string.h>
#include <stdio.h>

#include "sensor_button.hpp"
#include "../external/wiringPi/wiringPi.h"


void* button_working_thread(
    void* p_arg)
{
    sensor_button_class* p_button = (sensor_button_class*)p_arg;
    unsigned char sensor_gpio_num = p_button->get_gpio_num();
    void* p_user_data = NULL;
    button_up_cb p_button_up_cb = p_button->get_callback(&p_user_data);

    printf("Waiting for button ... ");
    while(digitalRead(sensor_gpio_num) == HIGH)
        delay(100);
    printf ("button pressed\n") ;
    while(digitalRead(sensor_gpio_num) == LOW)
        delay(100);
    printf ("button released\n") ;

    p_button_up_cb(p_user_data);
    p_button->activate();

    return NULL;
}

sensor_button_class::sensor_button_class(
    unsigned char gpio_num,
    const char* p_name)
{
    sensor_gpio_num = gpio_num;
    strcpy(p_sensor_name, p_name);
}

sensor_button_class::~sensor_button_class(
    void)
{
    pthread_cancel(pth);

    printf("wait for join\n");
    // wait for our thread to finish before continuing
    pthread_join(pth, NULL);

    printf("button destroyed\n");
}

void sensor_button_class::activate(
        void)
{
    pinMode(sensor_gpio_num, INPUT);

    // Create worker thread
    pthread_create(&pth, NULL, button_working_thread, this);
    printf("button working thread create\n");
}
