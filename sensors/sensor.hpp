
#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <string.h>

class sensor_class
{
protected:
    unsigned char sensor_gpio_num;
    char p_sensor_name[128];

public:

    sensor_class (
        unsigned char gpio_num,
        const char* p_name)
    {
        sensor_gpio_num = gpio_num;
        strcpy(p_sensor_name, p_name);
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

    virtual void activate (
        void) = 0;
};

typedef int (*trigger_cb) (
    const void* p_user_data_);

class sensor_trigger_class : public sensor_class
{
protected:
    trigger_cb p_trigger_cb = NULL;
    void* p_user_data = NULL;

public:

    sensor_trigger_class (
        unsigned char gpio_num,
        const char* p_name) : sensor_class(gpio_num, p_name)
    {
        //sensor_gpio_num = gpio_num;
        //strcpy(p_sensor_name, p_name);
    }

    void set_trigger_cb (
        trigger_cb p_trigger_cb_,
        const void* p_user_data_)
    {
        p_trigger_cb = p_trigger_cb_;
        p_user_data = (void*)p_user_data_;

    }

    trigger_cb get_callback (
        void** p_user_data_)
    {
        *p_user_data_ = p_user_data;
        return p_trigger_cb;
    }
};

#endif // SENSOR_HPP
 
