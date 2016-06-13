
#include "message.hpp"
#include "debug.hpp"
#include "socket.hpp"

int main (
    int argc,
    char **argv)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    unsigned char p_buffer [128];
    unsigned char* p_header_ptr = p_buffer;
    unsigned char* p_message_ptr = &p_buffer [sizeof (message_class::message_header_t)];

    server_socket_class server_socket;
    result = server_socket.create_fd ();

    while (result == RESULT_OK)
    {
        //message_header_t header;

        result = server_socket.accept_client ();

        if (result == RESULT_OK)
        {
            result = server_socket.recv_data (p_header_ptr, sizeof (message_class::message_header_t));
            if (result == RESULT_TRANSPORT_ERROR)
            {
                DEBUG_LOG_MESSAGE ("socket transport error. reset the existing connection");
                server_socket.close_client ();
                result = RESULT_OK;
                continue;
            }
        }

        if (result == RESULT_OK)
        {
            message_class::message_header_t* p_header = (message_class::message_header_t*) p_header_ptr;

            result = server_socket.recv_data (p_message_ptr, p_header->payload_size);
            if (result == RESULT_TRANSPORT_ERROR)
            {
                DEBUG_LOG_MESSAGE ("socket transport error. reset the existing connection");
                server_socket.close_client ();
                result = RESULT_OK;
                continue;
            }
        }

        if (result == RESULT_OK)
        {
            server_socket.close_client ();
        }

        if (result == RESULT_OK)
        {
            message_class::message_header_t* p_header = (message_class::message_header_t*) p_header_ptr;
            unsigned int message_length = sizeof (message_class::message_header_t) + p_header->payload_size;
            DEBUG_LOG_PRINT_ARRAY ("message", p_buffer, message_length);
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}
