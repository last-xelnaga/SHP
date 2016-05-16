
#ifndef SENSOR_EVENT_HPP
#define SENSOR_EVENT_HPP

#include "sensor.hpp"
#include <string.h>
#include <pthread.h>

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
            const char * p_sensor_type_) :
            sensor_class (sensor_gpio_num_, p_sensor_name_, p_sensor_type_)
    {
        pth = 0;
    }

public:
    virtual void set_event_callback (
            f_event_callback p_event_on_callback_,
            f_event_callback p_event_off_callback_,
            const sensor_manager_class* p_sensor_manager_)
    {
        p_event_on_callback = p_event_on_callback_;
        p_event_off_callback = p_event_off_callback_;
        p_sensor_manager = (sensor_manager_class*) p_sensor_manager_;
    }

    void event_on (
            void)
    {
        if (p_event_on_callback)
            p_event_on_callback (p_sensor_manager, this);
    }

    void event_off (
            void)
    {
        if (p_event_off_callback)
            p_event_off_callback (p_sensor_manager, this);
    }

    virtual ~sensor_event_class (
            void)
    {
        pthread_cancel (pth);
        //printf ("wait for join\n");
        // wait for our thread to finish before continuing
        pthread_join (pth, NULL);
        //printf ("button destroyed\n");
    }
};

#endif // SENSOR_EVENT_HPP
