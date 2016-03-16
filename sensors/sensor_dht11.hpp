
#ifndef SENSOR_DHT11_HPP
#define SENSOR_DHT11_HPP

#include "sensor.hpp"
#include <pthread.h>


class sensor_dht11_class : public sensor_class
{

    void prepare_bus (
        void);

    void get_data (
        void);

public:
    sensor_dht11_class (
        unsigned char gpio_num,
        const char* p_name);

    virtual ~sensor_dht11_class (
        void);

    virtual void activate (
        void);
    void show (
        void);
};



#endif // SENSOR_DHT11_HPP
 
