
#include "queue.hpp"
#include "message.hpp"
#include "debug.hpp"
#include <pthread.h>

static pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER;


queue_class::queue_class (
        void)
{
    message_id = 0;
}

error_code_t queue_class::peek (
        unsigned int* id,
        message_class** p_message)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // enter the critical section, lock the mutex
    pthread_mutex_lock (&map_mutex);

    if (message_map.size ())
    {
        message_map_t::iterator it = message_map.begin ();
        *id = it->first;
        *p_message = it->second;
    }
    else
    {
        *id = 0;
    }

    // we finished with critical sectio, so unlock the mutex
    pthread_mutex_unlock (&map_mutex);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t queue_class::remove (
        const unsigned int id)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // enter the critical section, lock the mutex
    pthread_mutex_lock (&map_mutex);

    message_map_t::iterator it = message_map.find (id);
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

error_code_t queue_class::add (
        message_class* const p_message)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // enter the critical section, lock the mutex
    pthread_mutex_lock (&map_mutex);

    message_map.insert (std::pair <int, message_class*> (++ message_id, p_message));

    // we finished with critical sectio, so unlock the mutex
    pthread_mutex_unlock (&map_mutex);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

queue_class::~queue_class (
        void)
{
    // enter the critical section, lock the mutex
    pthread_mutex_lock (&map_mutex);

    message_map_t::iterator it;
    for (it = message_map.begin (); it != message_map.end (); ++ it)
    {
        delete it->second;
    }

    message_map.clear ();

    // we finished with critical sectio, so unlock the mutex
    pthread_mutex_unlock (&map_mutex);
}
