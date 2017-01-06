
#include "proxy_main.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "debug.hpp"
#include "socket_server.hpp"
#include "socket_client.hpp"
#include <stdio.h>
#include <string.h>


error_code_t process_message_command (
        message_class* const p_message,
        server_socket_class* const p_server_socket,
        config_t* config)
{
    error_code_t result = RESULT_OK;
    printf ("process_message_command\n");

    if (p_message) {}
    if (config) {}

    /*client_socket_class socket;
    message_class message (message_class::send_command);

    message_class* p_answer = NULL;
    if (socket.connect ("127.0.0.1", 3460) == RESULT_OK)
    {
        if (socket.send_and_receive (&message, &p_answer) == RESULT_OK)
        //if (socket.send_message (&message) == RESULT_OK)
        {
            //process_message_configuration (p_answer, &config);
        }
    }*/

    message_class message_rep (message_class::send_command_result);
    /*result = */p_server_socket->send_message (&message_rep);

    return result;
}
