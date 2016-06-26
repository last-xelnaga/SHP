
#include "config.hpp"
#include "sensor_manager.hpp"
#include "debug.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "socket_client.hpp"

#include <stdio.h>
#include <unistd.h>

// states
RESET // create socket
VERSION // ask for version
CONFIG // send config, create server socket
READY // skip if not server socket
PROCESS // process server messages, process messages
IDLE // sleep



static config_class* config (
        void)
{
   return config_class::instance ();
}

error_code_t send_message (
        client_socket_class* const p_socket,
        message_class* const p_message)
{
    error_code_t result = RESULT_OK;
    message_class* p_answer = NULL;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        result = p_socket->connect (config ()->network.address.c_str (),
                config ()->network.port);
    }

    if (result == RESULT_OK)
    {
        result = p_socket->send_and_receive (p_message, &p_answer);
    }

    if (result == RESULT_OK)
    {
        // check the answer
    }

    p_socket->close ();

    delete p_answer;

    DEBUG_LOG_TRACE_END (result)
    return result;
}

int main (
        int argc,
        char **argv)
{
    error_code_t result = RESULT_OK;
    sensor_manager_class* p_sensor_manager = NULL;
    queue_class* p_queue = NULL;
    client_socket_class* p_socket = NULL;
    DEBUG_LOG_TRACE_BEGIN

    // create and read config
    if (result == RESULT_OK)
    {
        result = config ()->read_config ("shp_client.cfg");
    }

    // organize a queue
    if (result == RESULT_OK)
    {
        p_queue = new queue_class ();
    }

    // create sensor manager, initialize and verify settings
    if (result == RESULT_OK)
    {
        p_sensor_manager = new sensor_manager_class ();
        p_sensor_manager->setup_sensors (p_queue);
    }

    // create socket
    if (result == RESULT_OK)
    {
        p_socket = new client_socket_class ();
    }

    // check version
    if (result == RESULT_OK)
    {
        message_class* p_message = new message_class (message_class::send_version);
        result = p_message->add_string_to_message (message_class::version, "0.1");

        if (result == RESULT_OK)
        {
            result = send_message (p_socket, p_message);
        }

        if (result == RESULT_OK)
        {
            // TODO
        }

        delete p_message;
    }

    // send config
    if (result == RESULT_OK)
    {
        message_class* p_message = new message_class (message_class::send_configuration);

        p_message->add_string_to_message (message_class::config_ip, "192.168.1.1");
        p_message->add_num_to_message (message_class::config_port, 3654);
        p_message->add_string_to_message (message_class::config_name, config ()->general.name.c_str ());
        p_message->add_num_to_message (message_class::config_sensors, config ()->sensors.size ());

        std::vector <sensor_settings_t>::const_iterator sensor;
        for (sensor = config ()->sensors.begin (); sensor != config ()->sensors.end () && result == RESULT_OK; ++ sensor)
        {

            /*result = */p_message->add_string_to_message (message_class::sensor_name, sensor->name.c_str ());
            /*result = */p_message->add_string_to_message (message_class::sensor_type, sensor->type.c_str ());

        }

        if (result == RESULT_OK)
        {
            result = send_message (p_socket, p_message);
        }

        if (result == RESULT_OK)
        {
            // TODO
        }

        delete p_message;
    }

    if (result == RESULT_OK)
    {
        p_sensor_manager->activate_sensors ();
    }

    // loop
    if (result == RESULT_OK)
    {
        //int x = 100;

        while (result == RESULT_OK /*&& x >= 0*/)
        {
            unsigned int message_id = 0;
            message_class* p_message = NULL;

            result = p_queue->peek (&message_id, &p_message);
            if (message_id == 0)
            {
                usleep (500000);
                continue;
            }

            if (result == RESULT_OK)
            {
                result = send_message (p_socket, p_message);
            }

            if (result == RESULT_OK)
            {
                p_queue->remove (message_id);
                //x --;
            }
        }
    }

    delete config_class::instance ();
    delete p_sensor_manager;
    delete p_queue;
    delete p_socket;

    DEBUG_LOG_TRACE_END (result)
    return result;
}
