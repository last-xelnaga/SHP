
#include "message.hpp"
#include "queue.hpp"
#include "debug.hpp"
#include "socket_server.hpp"

#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
    char name [16];
    char buffer[64];
    time_t last_message;
} client_conf_t;

static server_socket_class* p_server_socket = NULL;
static client_conf_t screen_buf [3];

void* working_thread (
        void* p_arg)
{
    error_code_t result = RESULT_OK;
    queue_class* p_queue = (queue_class*) p_arg;

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
            p_queue->add (p_message);

            message_class message (message_class::send_event_result);
            result = p_server_socket->send_message (&message);
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

void process_message (
        message_class* p_message)
{
    char p_buffer [64] = { 0 };

    unsigned int payload_size = 0;
    unsigned char* p_payload = NULL;
    p_message->get_field_from_message (message_class::sensor_name, &payload_size, &p_payload);
    char* p_name = (char*) p_payload;

    p_message->get_field_from_message (message_class::sensor_data, &payload_size, &p_payload);
    char* p_event = (char*) p_payload;

    p_message->get_field_from_message (message_class::message_time, &payload_size, &p_payload);
    time_t event_time;
    memcpy (&event_time, p_payload, sizeof (event_time));
    struct tm* event_tm = localtime (&event_time);

    sprintf (p_buffer, "%-10s %.2i:%.2i:%.2i", (!strcmp (p_event, "1") ? "alarm" : ""), event_tm->tm_hour, event_tm->tm_min, event_tm->tm_sec);

    for (int i = 0; i < 3; ++ i)
    {
        if (!strcmp (screen_buf[i].name, p_name))
        {
            strcpy (screen_buf[i].buffer, p_buffer);
            screen_buf[i].last_message = time (NULL);
        }
    }
}

void show_screen (
        void)
{
    time_t now;
    time (&now);

    for (int i = 0; i < 3; ++ i)
    {
        if (screen_buf[i].last_message)
            printf ("%-16s %-20s %lu s     \n", screen_buf[i].name, screen_buf[i].buffer, now - screen_buf[i].last_message);
        else
            printf ("%-16s %-20s %s        \n", screen_buf[i].name, screen_buf[i].buffer, "never");
    }
    //http://stackoverflow.com/questions/26423537/how-to-position-the-input-text-cursor-in-c
    printf("\033[3A\r");
}

int main (
        int argc,
        char **argv)
{
    error_code_t result = RESULT_OK;
    pthread_t pth;
    DEBUG_LOG_TRACE_BEGIN

    p_server_socket = new server_socket_class ();
    result = p_server_socket->bind (3456);

    printf ("%-16s %-20s %s\n", "NAME", "DATA", "ELAPSED");
    strcpy( screen_buf[0].name, "motion");
    memset( screen_buf[0].buffer, 0, 64);
    screen_buf[0].last_message = 0;

    strcpy( screen_buf[1].name, "motion 2");
    memset( screen_buf[1].buffer, 0, 64);
    screen_buf[1].last_message = 0;

    strcpy( screen_buf[2].name, "motion 3");
    memset( screen_buf[2].buffer, 0, 64);
    screen_buf[2].last_message = 0;

    // organize a queue
    queue_class* p_queue = new queue_class ();
    pthread_create (&pth, NULL, working_thread, p_queue);

    // loop
    if (result == RESULT_OK)
    {
        //int x = 10;

        while (result == RESULT_OK /*&& x >= 0*/)
        {
            unsigned int message_id = 0;
            message_class* p_message = NULL;

            show_screen ();

            result = p_queue->peek (&message_id, &p_message);
            if (message_id == 0)
            {
                usleep (500000);
                continue;
            }

            if (result == RESULT_OK)
            {
                process_message (p_message);
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
