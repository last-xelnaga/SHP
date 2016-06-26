
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include "error_codes.hpp"

struct config_t;

typedef struct general_settings
{
    //char p_name [NAME_LENGTH + 1];
    std::string name;
} general_settings_t;

typedef struct network_settings
{
    //char p_address [IP4_ADDRESS_LENGTH + 1];
    std::string address;
    unsigned int port;
    unsigned int keep_alive_timeout;
    unsigned int connect_retry_count;
    unsigned int connect_retry_sleep;
    unsigned int write_timeout;
    unsigned int read_timeout;
} network_settings_t;

typedef struct sensor_settings
{
    //char p_name [NAME_LENGTH + 1];
    std::string name;
    unsigned int gpio;
    //char p_type [TYPE_LENGTH + 1];
    std::string type;
} sensor_settings_t;

class config_class
{
private:
    static config_class* p_instance;

public:
    // general
    general_settings_t general;

    // network settings
    network_settings_t network;

    // sensors
    std::vector <sensor_settings_t> sensors;

private:
    config_class (
            void);

    error_code_t read_string (
            config_t* p_cfg,
            const char* p_param_name,
            /*char* p_holder*/std::string* p_holder,
            unsigned int holder_length);

    error_code_t read_number (
            config_t* p_cfg,
            const char* p_param_name,
            unsigned int* p_holder);

    error_code_t read_general (
            config_t* p_cfg);

    error_code_t read_network (
            config_t* p_cfg);

    error_code_t read_sensors (
            config_t* p_cfg);

public:
    static config_class* instance (
            void)
    {
        if (!p_instance)
            p_instance = new config_class ();
        return p_instance;
    }

    error_code_t read_config (
            const char* p_cfg_filename);

    ~config_class (
            void);
};

#endif // CONFIG_HPP
