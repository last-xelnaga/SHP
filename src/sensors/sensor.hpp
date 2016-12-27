
#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <string.h>

class sensor_class
{
protected:
    unsigned char sensor_gpio_num;
    char p_sensor_name [64];
    char p_sensor_type [16];
    volatile bool activated;

public:

    sensor_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_,
            const char * p_sensor_type_)
    {
        sensor_gpio_num = sensor_gpio_num_;
        strncpy (p_sensor_name, p_sensor_name_, sizeof (p_sensor_name));
        strncpy (p_sensor_type, p_sensor_type_, sizeof (p_sensor_type));
        activated = false;
    }

    virtual ~sensor_class (
            void)
    {

    }

    unsigned char get_gpio_num (
            void)
    {
        return sensor_gpio_num;
    }

    const char* get_name (
            void)
    {
        return p_sensor_name;
    }

    const char* get_type (
            void)
    {
        return p_sensor_type;
    }

    virtual void activate (
            void) = 0;

    bool is_active (
            void)
    {
        return activated;
    }
};

#endif // SENSOR_HPP
