
#include "client_main.hpp"
#include "queue.hpp"
#include "client_config.hpp"
#include "debug.hpp"
#include "message.hpp"
#include "socket_client.hpp"
#include <stdlib.h>


error_code_t process_event (
        queue_class* const p_queue)
{
    error_code_t result = RESULT_OK;
    client_socket_class socket;
    DEBUG_LOG_TRACE_BEGIN

    unsigned int message_id = 0;
    message_class* p_message = NULL;
    result = p_queue->peek (&message_id, &p_message);
    if (message_id != 0)
    {
        message_class* p_answer = NULL;

        if (result == RESULT_OK)
        {
            result = socket.connect (get_network_settings ()->p_address,
                    get_network_settings ()->port);
        }

        //process_message
        if (result == RESULT_OK)
        {
            result = socket.send_and_receive (p_message, &p_answer);
        }

        if (result == RESULT_OK)
        {
            // check the answer
        }

        if (result == RESULT_OK)
        {
            p_queue->remove (message_id);
        }

        socket.close ();

        delete p_answer;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}
