
#ifndef SENSOR_FLAME_HPP
#define SENSOR_FLAME_HPP

#include "sensor_event.hpp"


class sensor_flame_class : public sensor_event_class
{
public:
    sensor_flame_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_);

    virtual void activate (
            void);
};

#endif // SENSOR_FLAME_HPP
