
#include "socket_server.hpp"
#include "message.hpp"
#include "debug.hpp"

#include <unistd.h>


server_socket_class::server_socket_class (
        void)
{
    socket_fd = -1;
    accept_fd = -1;

    write_timeout = 10;
    read_timeout = 60;
}

error_code_t server_socket_class::bind (
        unsigned int port_number)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        result = create_socket (&socket_fd);
    }

    if (result == RESULT_OK)
    {
        result = bind_on_server (socket_fd, port_number);
    }

    // make a clean up if needed
    if (result != RESULT_OK)
    {
        close_socket (socket_fd);
        socket_fd = -1;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t server_socket_class::accept_client (
        void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (socket_fd < 0)
    {
        DEBUG_LOG_MESSAGE ("socket is not ready");
        result = RESULT_INVALID_STATE;
    }

    // set blocking mode
    if (result == RESULT_OK)
    {
        result = set_blocking (socket_fd, false);
    }

    if (result == RESULT_OK)
    {
        result = accept (socket_fd, &accept_fd);
    }

    // set non-blocking mode
    if (result == RESULT_OK)
    {
        result = set_blocking (socket_fd, true);
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t server_socket_class::recv_message (
        message_class** p_message)
{
    error_code_t result = RESULT_OK;
    message_class::message_header_t header;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        result = recv_data (accept_fd, (unsigned char*) &header, sizeof (header), read_timeout);
    }

    if (result == RESULT_OK)
    {
        *p_message = new message_class (header);
    }

    if (result == RESULT_OK)
    {
        unsigned char* p_data = NULL;
        unsigned int data_length = 0;
        (*p_message)->get_payload (&p_data, &data_length);
        if (data_length)
        {
            result = recv_data (accept_fd, p_data, data_length, read_timeout);
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t server_socket_class::send_message (
        message_class* p_message)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        unsigned char* p_data = NULL;
        unsigned int data_length = 0;
        p_message->get_header (&p_data, &data_length);
        result = send_data (accept_fd, p_data, data_length, write_timeout);
    }

    if (result == RESULT_OK)
    {
        unsigned char* p_data = NULL;
        unsigned int data_length = 0;
        p_message->get_payload (&p_data, &data_length);
        if (data_length)
        {
            result = send_data (accept_fd, p_data, data_length, write_timeout);
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

void server_socket_class::close_client (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    close_socket (accept_fd);
    accept_fd = -1;

    DEBUG_LOG_TRACE_END (RESULT_OK)
}

void server_socket_class::close (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    close_client ();
    close_socket (socket_fd);
    socket_fd = -1;

    DEBUG_LOG_TRACE_END (RESULT_OK)
}

server_socket_class::~server_socket_class (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    server_socket_class::close ();

    DEBUG_LOG_TRACE_END (RESULT_OK)
}
