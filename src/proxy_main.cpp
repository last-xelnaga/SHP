
#include "proxy_main.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "debug.hpp"
#include "socket_server.hpp"
#include "socket_client.hpp"
#include <unistd.h>
#include <pthread.h>
#include <string.h>


static config_t config =
{
    0, // client_id
    "", // p_name
    0, // event_time
    0, // sensors_count
    NULL, // p_sensors
    NULL // p_screen
};

typedef struct
{
    queue_class* p_queue;
    server_socket_class* p_server_socket;
} thread_args_t;

void* working_thread (
        void* p_arg)
{
    error_code_t result = RESULT_OK;
    thread_args_t* p_thread_args = (thread_args_t*) p_arg;
    queue_class* p_queue = p_thread_args->p_queue;
    server_socket_class* p_server_socket = p_thread_args->p_server_socket;

    while (result == RESULT_OK)
    {
        message_class* p_message = NULL;
        result = p_server_socket->accept_client ();

        if (result == RESULT_OK)
        {
            result = p_server_socket->recv_message (&p_message);
        }

        if (result == RESULT_OK)
        {
            switch (p_message->get_message_id ())
            {
                case message_class::send_version:
                {
                    process_message_version (p_message, p_server_socket);
                    delete p_message;
                    break;
                }
                case message_class::send_config:
                {
                    process_message_configuration (p_message, p_server_socket, &config);
                    delete p_message;
                    break;
                }
                case message_class::send_event:
                {
                    process_message_event (p_message, p_server_socket, p_queue);
                    break;
                }
                case message_class::send_register:
                {
                    process_message_register (p_message, p_server_socket, &config);
                    delete p_message;
                    break;
                }
                case message_class::send_command:
                {
                    process_message_command (p_message, p_server_socket, &config);
                    delete p_message;
                    break;
                }
                default:
                {
                    DEBUG_LOG_MESSAGE ("message id is unknown");
                    break;
                }
            }
        }

        if (result != RESULT_OK)
        {
            DEBUG_LOG_MESSAGE ("socket transport error. skip");
            result = RESULT_OK;
        }

        p_server_socket->close_client ();
    }

    return NULL;
}

int main (
        int argc,
        char **argv)
{
    if (argc){}
    if (argv){}
    error_code_t result = RESULT_OK;
    pthread_t pth;
    DEBUG_LOG_TRACE_BEGIN

    server_socket_class* p_server_socket = new server_socket_class ();
    result = p_server_socket->bind (3456);

    // organize a queue
    queue_class* p_queue = new queue_class ();

    thread_args_t thread_args = {p_queue, p_server_socket};

    pthread_create (&pth, NULL, working_thread, &thread_args);

    // loop
    if (result == RESULT_OK)
    {
        //int x = 10;

        while (result == RESULT_OK /*&& x >= 0*/)
        {
            unsigned int message_id = 0;
            message_class* p_message = NULL;

            //show_screen ();

            result = p_queue->peek (&message_id, &p_message);
            if (message_id == 0)
            {
                usleep (500000);
                continue;
            }

            {
                client_socket_class socket;
                message_class* p_answer = NULL;
                if (socket.connect ("127.0.0.1", 3457) == RESULT_OK)
                    socket.send_and_receive (p_message, &p_answer);
                delete p_answer;
            }

            if (result == RESULT_OK)
            {
                //show_message_event (p_message);
                p_queue->remove (message_id);
                //x --;
            }
        }
    }

    pthread_cancel (pth);
    pthread_join (pth, NULL);

    delete p_queue;

    p_server_socket->close ();
    delete p_server_socket;

    DEBUG_LOG_TRACE_END (result)
    return result;
}
