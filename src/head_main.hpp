
#ifndef PROXY_MAIN_HPP
#define PROXY_MAIN_HPP

#include "error_codes.hpp"
#include <time.h>

#define NAME_LENGTH             32
#define IP4_ADDRESS_LENGTH      15
#define TYPE_LENGTH             16


typedef struct
{
    char p_name [NAME_LENGTH + 1];
    char p_type [TYPE_LENGTH + 1];
    unsigned int gpio;
} __attribute__((packed)) sensor_config_t;

typedef struct
{
    unsigned int gpio;
    char name [NAME_LENGTH + 1];
    char buffer[64];
    time_t last_message;
} sensor_status_t;

typedef struct
{
    unsigned int client_id;
    char p_name [NAME_LENGTH + 1];
    time_t event_time;

    unsigned int sensors_count;
    sensor_config_t* p_sensors;
    sensor_status_t* p_screen;
} config_t;

class message_class;
class server_socket_class;
class queue_class;


error_code_t process_message_configuration(
        message_class* const p_message,
        config_t* p_config);

void process_message_event (
        message_class* const p_message,
        server_socket_class* const p_server_socket,
        queue_class* p_queue);

#endif // PROXY_MAIN_HPP
