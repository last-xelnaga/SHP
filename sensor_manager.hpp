
#ifndef SENSOR_MANAGER_HPP
#define SENSOR_MANAGER_HPP

//#include <vector>
//#include "sensors/sensor.hpp"
#include "error_codes.hpp"

class sensor_manager_class
{
private:
    static sensor_manager_class* p_instance;

    bool is_initialized;

    //std::vector <sensor_settings_t> sensors;

private:
    error_code_t setup_board (
            void);

public:
    sensor_manager_class (
            void);

    /*static sensor_manager_class* instance (
        void)
    {
        if (!p_instance)
            p_instance = new sensor_manager_class();
        return p_instance;
    }*/

    error_code_t add_sensor (
            const char* p_name,
            const unsigned int gpio,
            const char* p_type);
};

#endif // SENSOR_MANAGER_HPP
