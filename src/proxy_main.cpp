
#include "proxy_main.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "debug.hpp"
#include "socket_server.hpp"
#include <unistd.h>
#include <pthread.h>
#include <string.h>


static server_socket_class* p_server_socket = NULL;

static config_t config =
{
    0, // client_id
    "", // p_name
    0, // event_time
    0, // sensors_count
    NULL // p_sensors
};


static void show_message_event (
        message_class* const p_message)
{
    char p_buffer [64] = { 0 };

    unsigned int payload_size = 0;
    unsigned char* p_payload = NULL;
    //p_message->get_field_from_message(message_class::sensor_name, &payload_size, &p_payload);
    //char* p_name = (char*)p_payload;

    p_message->get_field_from_message (message_class::sensor_data, &payload_size, &p_payload);
    char* p_event = (char*) p_payload;

    p_message->get_field_from_message (message_class::message_time, &payload_size, &p_payload);
    time_t event_time;
    memcpy (&event_time, p_payload, sizeof (event_time));
    struct tm* event_tm = localtime (&event_time);

    //p_message->get_field_from_message(message_class::sensor_type, &payload_size, &p_payload);
    //char* p_type = (char*)p_payload;

    unsigned int gpio = 0;
    p_message->get_field_from_message (message_class::sensor_gpio, &payload_size, &p_payload);
    memcpy (&gpio, p_payload, sizeof (gpio));

    sprintf (p_buffer, "%-10s %.2i:%.2i:%.2i", (!strcmp (p_event, "1") ? "alarm" : ""), event_tm->tm_hour, event_tm->tm_min, event_tm->tm_sec);

    for (unsigned int i = 0; i < config.sensors_count; i ++)
    {
        //DEBUG_LOG_MESSAGE("message event, gpio = %d, screen gpio = %d", gpio, screen_buf[i].gpio);

        if (gpio == config.p_screen[i].gpio)
        {
            strcpy (config.p_screen[i].buffer, p_buffer);
            config.p_screen[i].last_message = time (NULL);
        }
    }
}

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

void show_screen (
        void)
{
    time_t now;
    time(&now);

    if (config.sensors_count == 0)
        return;

    printf("%-16s %-20s %s\n", "NAME", "DATA", "ELAPSED");

    for (unsigned int i = 0; i < config.sensors_count; i ++)
    {
        if (config.p_screen[i].last_message)
            printf("%-16s %-20s %lu s     \n", config.p_screen[i].name, config.p_screen[i].buffer, now - config.p_screen[i].last_message);
        else
            printf("%-16s %-20s %s        \n", config.p_screen[i].name, config.p_screen[i].buffer, "never");
    }
    //http://stackoverflow.com/questions/26423537/how-to-position-the-input-text-cursor-in-c
    //printf("\033[3A\r");

    for (unsigned int i = 0; i < config.sensors_count + 1; i ++)
        printf("\033[1A\r");
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
                show_message_event (p_message);
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
