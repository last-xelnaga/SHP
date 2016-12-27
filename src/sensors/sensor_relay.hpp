
#ifndef SENSOR_RELAY_HPP
#define SENSOR_RELAY_HPP

#include "sensor.hpp"

class sensor_relay_class : public sensor_class
{
    int active;

public:
    sensor_relay_class (
        unsigned char gpio_num,
        const char* p_name);

    virtual ~sensor_relay_class (
        void);

    void switch_on (
            void);

    void switch_off (
            void);

    virtual void activate (
        void);
};

#endif // SENSOR_RELAY_HPP


