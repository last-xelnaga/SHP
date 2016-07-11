
#include "config.hpp"
#include "sensor_manager.hpp"
#include "debug.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "socket_client.hpp"
#include "version.hpp"

#include <unistd.h>


// states
const int RESET = 0;    // create socket
const int VERSION = 1; // ask for version
const int CONFIG = 2; // send config, create server socket
const int READY = 3; // skip if not server socket
const int PROCESS = 4; // process server messages, process messages
const int IDLE = 5; // sleep

static int current_state = RESET;


error_code_t process_version (
        client_socket_class* const p_socket,
        unsigned int revision);


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

error_code_t process_configuration  (
        client_socket_class* const p_socket)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

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


    LOG_MESSAGE ("shp client v%s", get_full_version ());

    // loop
    if (result == RESULT_OK)
    {
        //int x = 100;
        while (result == RESULT_OK /*&& x >= 0*/)
        {
            switch (current_state)
            {
                case RESET:
                {
                    DEBUG_LOG_MESSAGE ("state: RESET");

                    // create and read config
                    if (result == RESULT_OK)
                    {
                        result = config ()->read_config ("shp_client.cfg");
                    }

                    // create socket
                    if (result == RESULT_OK)
                    {
                        p_socket = new client_socket_class ();
                    }

                    if (result == RESULT_OK)
                    {
                        current_state = VERSION;
                    }
                    else
                    {
                        break;
                    }
                }
                case VERSION:
                {
                    DEBUG_LOG_MESSAGE ("state: VERSION");

                    // check version
                    if (result == RESULT_OK)
                    {
                        result = process_version (p_socket, get_revision ());
                    }

                    if (result == RESULT_OK)
                    {
                        current_state = CONFIG;
                    }
                    else
                    {
                        break;
                    }
                }
                case CONFIG:
                {
                    DEBUG_LOG_MESSAGE ("state: CONFIG");

                    // send config
                    if (result == RESULT_OK)
                    {
                        result = process_configuration (p_socket);
                    }

                    // create server socket
                    if (result == RESULT_OK)
                    {
                        // TODO
                    }

                    if (result == RESULT_OK)
                    {
                        current_state = READY;
                    }
                    else
                    {
                        break;
                    }
                }
                case READY:
                {
                    DEBUG_LOG_MESSAGE ("state: READY");

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
                        p_sensor_manager->activate_sensors ();
                    }

                    if (result == RESULT_OK)
                    {
                        current_state = PROCESS;
                        DEBUG_LOG_MESSAGE ("swich to PROCESS mode");
                    }
                    else
                    {
                        break;
                    }
                }
                case PROCESS:
                {
                    unsigned int message_id = 0;
                    message_class* p_message = NULL;

                    result = p_queue->peek (&message_id, &p_message);
                    if (message_id != 0)
                    {
                        //process_message
                        if (result == RESULT_OK)
                        {
                            result = send_message (p_socket, p_message);
                        }

                        if (result == RESULT_OK)
                        {
                            p_queue->remove (message_id);
                            //x --;
                        }
                        break;
                    }


                }
                case IDLE:
                {
                    usleep (500000);
                    //continue;
                }
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
