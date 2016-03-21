
#ifndef SENSOR_PIR_HPP
#define SENSOR_PIR_HPP

#include "sensor.hpp"
#include <pthread.h>


typedef int (*button_up_cb)(
        const void* p_user_data);

class sensor_pir_class : public sensor_class
{
    button_up_cb p_button_up_cb = NULL;
    void* p_user_data = NULL;
    pthread_t pth;  // this is our thread identifier

public:
    sensor_pir_class(
        unsigned char gpio_num,
        const char* p_name);

    virtual ~sensor_pir_class(void);

    void set_callback(
            button_up_cb p_callback,
            const void* p_data)
    {
        p_button_up_cb = p_callback;
        p_user_data = (void*)p_data;

    }

    button_up_cb get_callback(
        void** p_data)
    {
        *p_data = p_user_data;
        return p_button_up_cb;
    }

    virtual void activate(
            void);
};



#endif // SENSOR_PIR_HPP

