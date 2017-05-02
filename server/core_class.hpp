#ifndef CORE_CLASS_HPP
#define CORE_CLASS_HPP

#include <string>
#include <unordered_map>
#include <vector>

class core_class
{
private:
    struct sensor_class
    {
        unsigned char id;
        unsigned char type;
        std::string name;
        std::vector <unsigned char> payload;
    };

    struct probe_class
    {
        unsigned char id;
        std::string name;
        unsigned int time;
        int jitter;
        std::unordered_map <unsigned char, sensor_class> payload;
    };

    std::unordered_map <unsigned char, probe_class*> probes;


private:
    /*void notify_all_clients (
        void);*/

public:
    core_class (
        void);

    bool register_probe (
            const unsigned char probe_id,
            std::string probe_name,
            const unsigned int probe_time);

    bool register_sensor (
            const unsigned char probe_id,
            const unsigned char sensor_id,
            std::string sensor_name,
            const unsigned char sensor_type,
            const unsigned int event_time,
            std::vector <unsigned char> event_payload);

    /*void register_client (
        void);

    void process_command (
        void);
    void process_event (
        void);*/
};

#endif // CORE_CLASS_HPP
