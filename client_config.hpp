
#ifndef CLIENT_CONFIG_HPP
#define CLIENT_CONFIG_HPP

#include "error_codes.hpp"

#define NAME_LENGTH             32
#define IP4_ADDRESS_LENGTH      15
#define TYPE_LENGTH             16

typedef struct
{
    unsigned int client_id;
    char p_name [NAME_LENGTH + 1];
} general_settings_t;

typedef struct
{
    char p_address [IP4_ADDRESS_LENGTH + 1];
    unsigned int port;
    unsigned int connect_retry_count;
    unsigned int connect_retry_sleep;
    unsigned int write_timeout;
    unsigned int read_timeout;
    unsigned int keep_alive_timeout;
} network_settings_t;

typedef struct
{
    unsigned char is_initialized;
    unsigned int gpio;
    char p_name [NAME_LENGTH + 1];
    char p_type [TYPE_LENGTH + 1];

    // TODO
    // callbacks
    // settings
} sensor_settings_t;

typedef struct
{
    unsigned int sensors_count;
    sensor_settings_t* p_sensors;
} sensors_settings_t;

error_code_t read_config (
        void);

general_settings_t* get_general_settings (
        void);

network_settings_t* get_network_settings (
        void);

sensors_settings_t* get_sensor_settings (
        void);

void destroy_config (
        void);

#endif // CLIENT_CONFIG_HPP
