
#include "head_main.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "debug.hpp"
#include "socket_server.hpp"
#include <stdio.h>
#include <string.h>


void process_message_event (
        message_class* const p_message,
        server_socket_class* const p_server_socket,
        queue_class* p_queue)
{
    //DEBUG_LOG_MESSAGE ("process_message_event");
    p_queue->add (p_message);

    message_class message (message_class::send_event_result);
    /*result = */p_server_socket->send_message (&message);
}
