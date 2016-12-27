
#ifndef SENSOR_MANAGER_HPP
#define SENSOR_MANAGER_HPP

#include "error_codes.hpp"
#include <vector>

class queue_class;
class message_class;
class sensor_class;

class sensor_manager_class
{
private:
    bool is_initialized;

    queue_class* p_queue;

    std::vector <sensor_class*> sensors;

private:
    error_code_t setup_board (
            void);

    error_code_t add_sensor (
            const char* p_name,
            const unsigned int gpio,
            const char* p_type);

public:
    sensor_manager_class (
            void);

    error_code_t setup_sensors (
            queue_class* const p_queue_);

    error_code_t activate_sensors (
            void);

    void add_to_queue (
            message_class* const p_message);

    ~sensor_manager_class (
            void);
};

#endif // SENSOR_MANAGER_HPP
