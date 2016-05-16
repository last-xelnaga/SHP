
#include <unistd.h>
#include <pthread.h>
#include <map>

#include "queue_manager.hpp"
#include "message.hpp"
#include "socket.hpp"
#include "debug.hpp"

typedef std::map <unsigned int, message_class*> message_map_t;
static message_map_t message_map;
static unsigned int message_count;

static pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER;

queue_manager_class* queue_manager_class::p_instance = 0;

queue_manager_class::queue_manager_class (
        void)
{
    p_socket = 0;
    message_count = 0;
}

error_code_t queue_manager_class::send_message (
        message_class* p_message)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        result = p_socket->create_fd ();
    }

    if (result == RESULT_OK)
    {
        unsigned char* p_data = 0;
        unsigned int data_length = 0;
        p_message->get_header (&p_data, &data_length);
        result = p_socket->send_data (p_data, data_length);
    }

    if (result == RESULT_OK)
    {
        unsigned char* p_data = 0;
        unsigned int data_length = 0;
        p_message->get_payload (&p_data, &data_length);
        result = p_socket->send_data (p_data, data_length);
    }

    p_socket->close_fd ();

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t queue_manager_class::peek_message (
        unsigned int* message_id,
        message_class** p_message)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // enter the critical section, lock the mutex
    pthread_mutex_lock (&map_mutex);

    if (message_map.size ())
    {
        message_map_t::iterator it = message_map.begin ();
        *message_id = it->first;
        *p_message = it->second;
    }
    else
    {
        *message_id = 0;
    }

    // we finished with critical sectio, so unlock the mutex
    pthread_mutex_unlock (&map_mutex);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t queue_manager_class::remove_message (
        unsigned int message_id)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // enter the critical section, lock the mutex
    pthread_mutex_lock (&map_mutex);

    message_map_t::iterator it = message_map.find (message_id);
    if (it != message_map.end ())
    {
        delete (it->second);
        message_map.erase (it);
    }

    // we finished with critical sectio, so unlock the mutex
    pthread_mutex_unlock (&map_mutex);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t queue_manager_class::add_message (
        message_class* p_message)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // enter the critical section, lock the mutex
    pthread_mutex_lock (&map_mutex);

    message_map.insert (std::pair <int, message_class*> (++ message_count, p_message));

    // we finished with critical sectio, so unlock the mutex
    pthread_mutex_unlock (&map_mutex);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t queue_manager_class::run (
        client_socket_class* p_client_socket)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    p_socket = p_client_socket;

    while (result == RESULT_OK)
    {
        unsigned int message_id = 0;
        message_class* p_message = 0;

        sleep (2);
        //usleep (500000);

        result = peek_message (&message_id, &p_message);
        if (message_id == 0)
        {
            continue;
        }

        if (result == RESULT_OK)
        {
            result = send_message (p_message);
        }

        if (result == RESULT_OK)
        {
            remove_message (message_id);
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}
