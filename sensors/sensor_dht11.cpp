
#include "sensor_dht11.hpp"
#include "wiringPi.h"
#include <stdio.h>

#define DATA_LENGTH         5


void read_temperature (
        unsigned char sensor_gpio_num,
        unsigned char* p_data)
{
    // send 'start' command
    // pull the bus down
    digitalWrite (sensor_gpio_num, LOW);
    // and wait at least 18ms
    delay (30);

    // now pull the bus up
    digitalWrite (sensor_gpio_num, HIGH);
    // and wait for response (20 - 40 ms)
    delayMicroseconds (30);

    // at this point the sensor should start to react
    // on our 'start' command
    pinMode (sensor_gpio_num, INPUT);
    // sensor has to lower the bus, so wait for it
    while (digitalRead (sensor_gpio_num) == HIGH);
    // he has to keep low level for 80 ms
    delayMicroseconds (80);
    // and than put it high again. wait for it as well
    while (digitalRead (sensor_gpio_num) == LOW);
    // it will keep hight level for 80 ms, so skip them
    delayMicroseconds (80);

    // and it's ready to send 40 bits of data
    for (unsigned int i = 0; i < DATA_LENGTH; i ++)
    {
        // receive temperature and humidity data, the parity bit is not considered
        unsigned char byte = 0;
        for (int bit = 0; bit < 8; bit ++)
        {
            // wait for low bus. indicates that the transmission has started
            while (digitalRead (sensor_gpio_num) == HIGH);
            // wait for 50 ms of the low level. skip it
            while (digitalRead (sensor_gpio_num) == LOW);

            // now the sensor has switched the bus to high level
            // wait for 30 ms to check the bus level again
            delayMicroseconds (30);

            // check the bus again
            if (digitalRead (sensor_gpio_num) == HIGH)
            {
                // high bus after 30 ms means that we got '1',
                // since '1' requires 70 ms of high bus.
                // '0' should be finished within 26-28 ms.
                byte |= (1 << (7 - bit));
            }
        }
        p_data [i] = byte;
    }

    pinMode (sensor_gpio_num, OUTPUT);

    // pull the bus high. this one will show to sensor
    // that it could stay in low-power-consumption mode
    digitalWrite (sensor_gpio_num, HIGH);

    //printf("temp %s\t%i.%i, hum %i.%i, crc %i\n", p_sensor_name, data[0], data[1], data[2], data[3], data[4]);
}

int is_data_crc_valid (
        unsigned char* p_data)
{
    int result = -1;

    if (true)
    {
        result = 1;
    }

    return result;
}

void* dht11_working_thread (
        void* p_arg)
{
    sensor_dht11_class* p_sensor_event = (sensor_dht11_class*) p_arg;
    unsigned char sensor_gpio_num = p_sensor_event->get_gpio_num ();

    unsigned int retry_count = 3;

    unsigned char p_data [DATA_LENGTH];
    do
    {
        read_temperature (sensor_gpio_num, p_data);
        retry_count --;
    } while (is_data_crc_valid (p_data) != 1 && retry_count >= 0);

    p_sensor_event->event_off ();

    return NULL;
}

sensor_dht11_class::sensor_dht11_class (
        unsigned char gpio_num,
        const char* p_name) :
        sensor_event_class (gpio_num, p_name, "DHT11")
{

}

void sensor_dht11_class::activate (
        void)
{
    pinMode (sensor_gpio_num, OUTPUT);

    // pull the bus high. this one will show to sensor
    // that it could stay in low-power-consumption mode
    digitalWrite (sensor_gpio_num, HIGH);

    // create worker thread
    pthread_create (&pth, NULL, dht11_working_thread, this);
}
