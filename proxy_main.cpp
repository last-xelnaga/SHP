//#include <stdio.h>
//#include <unistd.h>
//#include <signal.h>

//#include "config.hpp"
//#include "sensor_manager.hpp"
//#include "queue_manager.hpp"
#include "debug.hpp"
#include "socket.hpp"

int main (
    int argc,
    char **argv)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    unsigned char p_buffer [128];
    //unsigned int message_length = 0;

    server_socket_class server_socket;
    server_socket.create_fd ();

    while (1)
    {
        server_socket.accept_client ();
        server_socket.recv_data (p_buffer, 1);
        //message_length = p_buffer[0];
        //server_socket.recv_data (&p_buffer[1], message_length);
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}
