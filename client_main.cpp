
#include "client_config.hpp"
#include "client_sensor_manager.hpp"
#include "debug.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "socket_client.hpp"
#include "version.hpp"
#include <unistd.h>


// states
const int RESET = 0;    // create socket
const int VERSION = 1;  // ask for version
const int CONFIG = 2;   // send config, create server socket
const int READY = 3;    // skip if not server socket
const int PROCESS = 4;  // process server messages, process messages
const int IDLE = 5;     // sleep

static int current_state = RESET;

error_code_t process_version (
        const unsigned int revision);

error_code_t process_configuration (
        void);

error_code_t send_message (
        client_socket_class* const p_socket,
        message_class* const p_message)
{
    error_code_t result = RESULT_OK;
    message_class* p_answer = NULL;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        result = p_socket->connect (get_network_settings ()->p_address,
                get_network_settings ()->port);
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
        char** argv)
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
                        result = read_config ();
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
                        result = process_version (get_revision ());
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

                    // create sensor manager, initialize and verify settings
                    if (result == RESULT_OK)
                    {
                        p_sensor_manager = new sensor_manager_class ();
                        p_queue = new queue_class ();
                        p_sensor_manager->setup_sensors (p_queue);
                    }

                    // send config
                    if (result == RESULT_OK)
                    {
                        result = process_configuration ();
                    }

                    if (result == RESULT_OK)
                    {
                        //current_state = IDLE;
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
                    //if (result == RESULT_OK)
                    //{
                    //    p_queue = new queue_class ();
                    //}

                    // create sensor manager, initialize and verify settings
                    if (result == RESULT_OK)
                    {
                        //p_sensor_manager = new sensor_manager_class ();
                        //p_sensor_manager->setup_sensors (p_queue);
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

    destroy_config ();
    delete p_sensor_manager;
    delete p_queue;
    delete p_socket;

    DEBUG_LOG_TRACE_END (result)
    return result;
}
