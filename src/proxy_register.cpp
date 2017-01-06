
#include "proxy_main.hpp"
#include "message.hpp"
#include "debug.hpp"
#include "socket_server.hpp"
#include <string.h>
#include <stdlib.h>


error_code_t process_message_register (
        message_class* const p_message,
        server_socket_class* const p_server_socket,
        config_t* p_config)
{
    error_code_t result = RESULT_OK;
    //DEBUG_LOG_MESSAGE ("message register");

    if (p_message) {}

    message_class message (message_class::send_register_result);
    message.add_time_to_message ();

    message.add_num_to_message (message_class::config_client_id, p_config->client_id);
    message.add_string_to_message (message_class::config_name, p_config->p_name);

    unsigned int sensor_count = p_config->sensors_count;
    message.add_num_to_message (message_class::config_sensor_count, sensor_count);
    //DEBUG_LOG_MESSAGE ("message register sensor_count = %d", sensor_count);

    if (sensor_count > 0)
    {
        message.add_field_to_message (message_class::config_sensor_data,
                (unsigned char*) p_config->p_sensors, sensor_count * sizeof (sensor_config_t));
    }

    result = p_server_socket->send_message (&message);

    return result;
}
