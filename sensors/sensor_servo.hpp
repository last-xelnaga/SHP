
#ifndef SENSOR_SERVO_HPP
#define SENSOR_SERVO_HPP

#include "sensor.hpp"

class sensor_servo_class : public sensor_class
{
    int active;

public:
    sensor_servo_class (
        unsigned char gpio_num,
        const char* p_name);

    virtual ~sensor_servo_class (
        void);

    void set_angle (
            unsigned char angle);

    virtual void activate (
        void);
};

#endif // SENSOR_SERVO_HPP


