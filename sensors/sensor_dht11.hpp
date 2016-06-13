
#ifndef SENSOR_DHT11_HPP
#define SENSOR_DHT11_HPP

#include "sensor_event.hpp"

#define DATA_LENGTH         5


class sensor_dht11_class : public sensor_event_class
{
private:
    unsigned char p_data [DATA_LENGTH];

private:
    void read_data (
            void);
    bool is_data_crc_valid (
            void);

public:
    sensor_dht11_class (
            unsigned char gpio_num,
            const char* p_name);

    void sensor_setup (
            void);

    void do_task (
            void);
};

#endif // SENSOR_DHT11_HPP
