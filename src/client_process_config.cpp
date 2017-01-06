
#include "client_main.hpp"
#include "debug.hpp"
#include "message.hpp"
#include "client_config.hpp"
#include "socket_client.hpp"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char p_name [NAME_LENGTH + 1];
    char p_type [TYPE_LENGTH + 1];
    unsigned int gpio;
} __attribute__((packed)) sensor_config_t;

static error_code_t prepare_config_message (
        message_class* const p_message)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    p_message->add_num_to_message (message_class::config_client_id, get_general_settings ()->client_id);
    p_message->add_string_to_message (message_class::config_name, get_general_settings ()->p_name);

    unsigned int sensor_count = get_sensor_settings ()->sensors_count;
    p_message->add_num_to_message (message_class::config_sensor_count, sensor_count);

    if (sensor_count > 0)
    {
        unsigned char* p_sensor_data = (unsigned char*) malloc (sensor_count * sizeof (sensor_config_t));
        for (unsigned int i = 0; i < sensor_count; i ++)
        {
            sensor_config_t* p_sensor_config = (sensor_config_t*) &p_sensor_data[i * sizeof (sensor_config_t)];
            strcpy (p_sensor_config->p_name, get_sensor_settings ()->p_sensors[i].p_name); //, NAME_LENGTH + 1);
            strcpy (p_sensor_config->p_type, get_sensor_settings ()->p_sensors[i].p_type);//, TYPE_LENGTH + 1);
            p_sensor_config->gpio = get_sensor_settings()->p_sensors[i].gpio;

            //LOG_MESSAGE("sensor %d, gpio %d, type %s, name %s", i, p_sensor_config->gpio, p_sensor_config->p_type, p_sensor_config->p_name);
        }

        p_message->add_field_to_message (message_class::config_sensor_data, p_sensor_data, sensor_count * sizeof (sensor_config_t));
        free (p_sensor_data);
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

static error_code_t process_answer (
    message_class* const p_answer)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // happy compiler
    if (p_answer) {}

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t process_configuration (
        void)
{
    error_code_t result = RESULT_OK;
    client_socket_class socket;
    DEBUG_LOG_TRACE_BEGIN

    // send config
    message_class message (message_class::send_config);
    if (result == RESULT_OK)
    {
        result = prepare_config_message (&message);
    }

    if (result == RESULT_OK)
    {
        result = socket.connect (get_network_settings ()->p_address,
                get_network_settings ()->port);
    }

    message_class* p_answer = NULL;
    if (result == RESULT_OK)
    {
        result = socket.send_and_receive (&message, &p_answer);
    }

    socket.close ();

    // check the answer
    if (result == RESULT_OK)
    {
        if (message_class::send_config_result != p_answer->get_message_id ())
        {
            LOG_MESSAGE ("wrong message id. waited for %d, but got %d",
                    message_class::send_config_result, p_answer->get_message_id ());
            result = RESULT_INVALID_STATE;
        }
    }

    if (result == RESULT_OK)
    {
        process_answer (p_answer);
    }

    delete p_answer;

    DEBUG_LOG_TRACE_END (result)
    return result;
}
