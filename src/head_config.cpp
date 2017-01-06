
#include "head_main.hpp"
#include "message.hpp"
#include "debug.hpp"
#include "socket_server.hpp"
#include <string.h>
#include <stdlib.h>


error_code_t process_message_configuration (
        message_class* const p_message,
        config_t* p_config)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_MESSAGE ("process_message_configuration");

    unsigned int payload_size = 0;
    unsigned char* p_payload = NULL;

    p_message->get_field_from_message (message_class::message_time, &payload_size, &p_payload);
    memcpy (&p_config->event_time, p_payload, sizeof (time_t));
    //DEBUG_LOG_MESSAGE("message config, time");

    unsigned int* client_id;
    p_message->get_field_from_message (message_class::config_client_id, &payload_size, (unsigned char**) &client_id);
    p_config->client_id = *client_id;
    //DEBUG_LOG_MESSAGE("message config, client_id = %d", p_config->client_id);

    p_message->get_field_from_message (message_class::config_name, &payload_size, &p_payload);
    strcpy (p_config->p_name, (char*) p_payload);
    //DEBUG_LOG_MESSAGE("message config, p_name = %s", p_config->p_name);

    unsigned int* sensors_count;
    p_message->get_field_from_message (message_class::config_sensor_count, &payload_size, (unsigned char**) &sensors_count);
    p_config->sensors_count = *sensors_count;
    //DEBUG_LOG_MESSAGE("message config, sensors_count = %d", p_config->sensors_count);

    if (p_config->sensors_count > 0)
    {
        p_message->get_field_from_message (message_class::config_sensor_data, &payload_size, &p_payload);
        if (payload_size != (p_config->sensors_count * sizeof (sensor_config_t)))
        {

        }
        p_config->p_sensors = (sensor_config_t*) malloc (payload_size);
        memcpy (p_config->p_sensors, p_payload, payload_size);

        p_config->p_screen = (sensor_status_t*) malloc (p_config->sensors_count * sizeof (sensor_status_t));
        for (unsigned int i = 0; i < p_config->sensors_count; i ++)
        {
            //DEBUG_LOG_MESSAGE ("get config, gpio=%d", p_config->p_sensors[i].gpio);
            //DEBUG_LOG_MESSAGE ("get config, p_name=%s", p_config->p_sensors[i].p_name);

            p_config->p_screen[i].gpio = p_config->p_sensors[i].gpio;
            strcpy (p_config->p_screen[i].name, p_config->p_sensors[i].p_name);
            memset (p_config->p_screen[i].buffer, 0, 64);
            p_config->p_screen[i].last_message = 0;
        }
    }

    return result;
}
