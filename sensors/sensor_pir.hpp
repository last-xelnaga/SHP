
#ifndef SENSOR_PIR_HPP
#define SENSOR_PIR_HPP

#include "sensor_event.hpp"


class sensor_pir_class : public sensor_event_class
{
public:
    sensor_pir_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_);

    virtual void activate (
            void);
};

#endif // SENSOR_PIR_HPP
