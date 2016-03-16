
#ifndef SENSOR_MANAGER_HPP
#define SENSOR_MANAGER_HPP

#include "sensor.hpp"

class sensor_manager_class
{
    static sensor_manager_class* p_instance;

    sensor_manager_class(void);

  public:

    static sensor_manager_class* instance(
        void)
    {
        if (!p_instance)
            p_instance = new sensor_manager_class();
        return p_instance;
    }

    int setup (
        void);

    void add();
};

#endif // SENSOR_MANAGER_HPP
