
#include "core_class.hpp"

core_class::core_class (
        void)
{

}

bool core_class::register_probe (
        const unsigned char probe_id,
        std::string probe_name,
        const unsigned int probe_time)
{
    bool result = true;

    try
    {
        probe_class* p_probe = new probe_class;
        p_probe->id = probe_id;
        p_probe->name = probe_name;
        p_probe->time = probe_time;
        p_probe->jitter = 0;

        probes [probe_id] = p_probe;
    }
    catch (...)
    {
        result = false;
    }

    return result;
}

bool core_class::register_sensor (
        const unsigned char probe_id,
        const unsigned char sensor_id,
        std::string sensor_name,
        const unsigned char sensor_type,
        const unsigned int event_time,
        std::vector <unsigned char> event_payload)
{
    bool result = true;

    try
    {

    }
    catch (...)
    {
        result = false;
    }

    return result;
}
