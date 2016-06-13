
#ifndef SENSOR_EVENT_HPP
#define SENSOR_EVENT_HPP

#include "sensor.hpp"
#include <string.h>
#include <sys/types.h>

class sensor_manager_class;
class sensor_event_class;

typedef void (*f_event_callback) (
        const sensor_manager_class* p_sensor_manager,
        const sensor_event_class* p_sensor);


class sensor_event_class : public sensor_class
{
protected:
    // this is our thread identifier
    pthread_t pth;

    f_event_callback p_event_on_callback = NULL;
    f_event_callback p_event_off_callback = NULL;
    sensor_manager_class* p_sensor_manager = NULL;

protected:

    sensor_event_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_,
            const char * p_sensor_type_);

public:
    void set_event_callback (
            f_event_callback p_event_on_callback_,
            f_event_callback p_event_off_callback_,
            const sensor_manager_class* p_sensor_manager_);

    void activate (
            void);

    void event_on (
            void);

    void event_off (
            void);

    virtual void do_task (
            void);

protected:
    virtual void sensor_setup (
            void);

    virtual ~sensor_event_class (
            void);
};

class sensor_button_class : public sensor_event_class
{
public:
    sensor_button_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_);
};

class sensor_flame_class : public sensor_event_class
{
public:
    sensor_flame_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_);
};

class sensor_pir_class : public sensor_event_class
{
public:
    sensor_pir_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_);

    virtual void do_task (
            void);
};

class sensor_reed_class : public sensor_event_class
{
public:
    sensor_reed_class (
            unsigned char sensor_gpio_num_,
            const char * p_sensor_name_);
};

#endif // SENSOR_EVENT_HPP
