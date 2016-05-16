
#ifndef SENSOR_DHT11_HPP
#define SENSOR_DHT11_HPP

#include "sensor_event.hpp"


class sensor_dht11_class : public sensor_event_class
{

public:
    sensor_dht11_class (
            unsigned char gpio_num,
            const char* p_name);

    virtual void activate (
            void);
};

#endif // SENSOR_DHT11_HPP
