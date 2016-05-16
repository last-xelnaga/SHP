
#ifndef SENSOR_BUTTON_HPP
#define SENSOR_BUTTON_HPP

#include "sensor_event.hpp"


class sensor_button_class : public sensor_event_class
{
public:
    sensor_button_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_);

    virtual void activate (
            void);
};

#endif // SENSOR_BUTTON_HPP
