
#ifndef SENSOR_SERVO_HPP
#define SENSOR_SERVO_HPP

#include "sensor.hpp"

class sensor_servo_class : public sensor_class
{
public:
    sensor_servo_class (
            unsigned char gpio_num,
            const char* p_name);

    void set_params (
            unsigned int min_time,
            unsigned int max_time,
            unsigned int frequence);

    void set_angle (
            unsigned char angle);

    virtual void activate (
            void);

    virtual ~sensor_servo_class (
            void);
private:
    int active;

    double wait_time;
    double k, b;
};

#endif // SENSOR_SERVO_HPP
