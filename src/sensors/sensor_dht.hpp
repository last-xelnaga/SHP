
#ifndef SENSOR_DHT_HPP
#define SENSOR_DHT_HPP

#include "sensor_event.hpp"

#define DATA_LENGTH         5


class sensor_dht_class : public sensor_event_class
{
public:
    sensor_dht_class (
            unsigned char gpio_num,
            const char* p_name);

    void do_task (
            void);

    virtual int get_temperature (
            void) = 0;

    virtual unsigned int get_humidity (
            void) = 0;

protected:
    unsigned char p_data [DATA_LENGTH];

protected:
    void sensor_setup (
            void);

    void read_data (
            void);

    virtual bool is_data_crc_valid (
            void) = 0;
};

class sensor_dht11_class : public sensor_dht_class
{
public:
    sensor_dht11_class (
            unsigned char gpio_num,
            const char* p_name);

    virtual int get_temperature (
            void);

    virtual unsigned int get_humidity (
            void);

protected:
    virtual bool is_data_crc_valid (
            void);
};

class sensor_dht22_class : public sensor_dht_class
{
public:
    sensor_dht22_class (
            unsigned char gpio_num,
            const char* p_name);

    virtual int get_temperature (
            void);

    virtual unsigned int get_humidity (
            void);

protected:
    virtual bool is_data_crc_valid (
            void);
};

#endif // SENSOR_DHT_HPP
