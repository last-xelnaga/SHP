
#include "socket_client.hpp"
#include "message.hpp"
#include "debug.hpp"

#include <unistd.h>


client_socket_class::client_socket_class (
        void)
{
    connect_retry_sleep = 2;
    try_count_max = 3;

    write_timeout = 10;
    read_timeout = 60;
}

error_code_t client_socket_class::connect (
        const char* p_server_name,
        unsigned int port_number)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        result = create_socket (&socket_fd);
    }

    // connect
    if (result == RESULT_OK)
    {
        result = RESULT_SOCKET_ERROR;
        unsigned int try_count = try_count_max;

        while (result != RESULT_OK && try_count)
        {
            result = connect_to_server (socket_fd, p_server_name, port_number);
            try_count --;

            if (result != RESULT_OK)
            {
                if (try_count == 0)
                {
                    // no attempts left
                    DEBUG_LOG_MESSAGE ("connect failed. no attempts left");
                    result = RESULT_SOCKET_ERROR;
                }
                else
                {
                    // give it an another chance
                    sleep (connect_retry_sleep);
                }
            }
        }
    }

    // set non-blocking mode
    if (result == RESULT_OK)
    {
        result = set_blocking (socket_fd, true);
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

error_code_t client_socket_class::send_and_receive (
        message_class* p_message,
        message_class** p_answer)
{
    error_code_t result = RESULT_OK;
    message_class::message_header_t header;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        unsigned char* p_data = NULL;
        unsigned int data_length = 0;
        p_message->get_header (&p_data, &data_length);
        result = send_data (socket_fd, p_data, data_length, write_timeout);
    }

    if (result == RESULT_OK)
    {
        unsigned char* p_data = NULL;
        unsigned int data_length = 0;
        p_message->get_payload (&p_data, &data_length);
        if (data_length)
        {
            result = send_data (socket_fd, p_data, data_length, write_timeout);
        }
    }

    if (result == RESULT_OK)
    {
        result = recv_data (socket_fd, (unsigned char*) &header, sizeof (header), read_timeout);
    }

    if (result == RESULT_OK)
    {
        *p_answer = new message_class (header);
    }

    if (result == RESULT_OK)
    {
        unsigned char* p_data = NULL;
        unsigned int data_length = 0;
        (*p_answer)->get_payload (&p_data, &data_length);
        if (data_length)
        {
            result = recv_data (socket_fd, p_data, data_length, read_timeout);
        }
    }

    DEBUG_LOG_TRACE_END (result);
    return result;
}

void client_socket_class::close (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    close_socket (socket_fd);
    socket_fd = -1;

    DEBUG_LOG_TRACE_END (RESULT_OK)
}

client_socket_class::~client_socket_class (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    client_socket_class::close ();

    DEBUG_LOG_TRACE_END (RESULT_OK)
}
