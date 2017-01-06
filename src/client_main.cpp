
#include "client_main.hpp"
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

int main (
        int argc,
        char** argv)
{
    if (argc){}
    if (argv){}
    error_code_t result = RESULT_OK;
    sensor_manager_class* p_sensor_manager = NULL;
    queue_class* p_queue = NULL;
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

                    // create sensor manager, initialize and verify settings
                    if (result == RESULT_OK)
                    {
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
                    if (result == RESULT_OK)
                    {
                        result = process_event (p_queue);
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

    DEBUG_LOG_TRACE_END (result)
    return result;
}
