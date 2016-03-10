
#ifndef SENSOR_HPP
#define SENSOR_HPP

//#include "../external/wiringPi/wiringPi.h"

class sensor_class
{
protected:
    unsigned char sensor_gpio_num;
    char p_sensor_name[128];

public:

    virtual ~sensor_class(void)
    {

    }

    unsigned char get_gpio_num(
            void)
    {
        return sensor_gpio_num;
    }

    const char* get_name(
            void)
    {
        return p_sensor_name;
    }

    virtual void activate(
            void) = 0;
};

#endif // SENSOR_HPP
 
