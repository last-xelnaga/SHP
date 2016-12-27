#include <gtest/gtest.h>
#include "socket.hpp"


void* server_thread (
        void* p_arg)
{
    unsigned char p_buffer[128];
    unsigned int message_length = 0;
    server_socket_class* p_server_socket = (server_socket_class*) p_arg;

    while (1)
    {
        p_server_socket->accept_client ();
        p_server_socket->recv_data (p_buffer, 1);
        message_length = p_buffer[0];
        p_server_socket->recv_data (&p_buffer[1], message_length);
        p_server_socket->send_data (p_buffer, message_length + 1);
    }

    return NULL;
}

void* deaf_server_thread (
        void* p_arg)
{
    unsigned char p_buffer[128];
    unsigned int message_length = 0;
    server_socket_class* p_server_socket = (server_socket_class*) p_arg;

    p_server_socket->accept_client ();
    p_server_socket->recv_data (p_buffer, 1);
    message_length = p_buffer[0];
    p_server_socket->recv_data (&p_buffer[1], message_length);

    return NULL;
}

void* channel_closed_server_thread (
        void* p_arg)
{
    unsigned char p_buffer[128];
    server_socket_class* p_server_socket = (server_socket_class*) p_arg;

    p_server_socket->accept_client ();
    p_server_socket->recv_data (p_buffer, 1);
    p_server_socket->close_client ();
    p_server_socket->close_fd ();

    return NULL;
}

TEST (socket_test, short_message_success)
{
    server_socket_class server_socket;
    server_socket.create_fd ();

    pthread_t pth;
    pthread_create (&pth, NULL, server_thread, &server_socket);

    client_socket_class client_socket;
    client_socket.create_fd ();

    unsigned char p_hello_buffer[] = { 6, 'H', 'E', 'L', 'L', 'O', 0 };
    unsigned int message_length = 7;
    unsigned char p_recv_buffer[7];

    client_socket.send_data (p_hello_buffer, message_length);
    client_socket.recv_data (p_recv_buffer, message_length);
    client_socket.close_fd ();

    EXPECT_EQ (0, memcmp (p_hello_buffer, p_recv_buffer, message_length));

    pthread_cancel (pth);
    pthread_join (pth, NULL);
    server_socket.close_client ();
    server_socket.close_fd ();
}

TEST (socket_test, long_message_success)
{
    server_socket_class server_socket;
    server_socket.create_fd ();

    pthread_t pth;
    pthread_create (&pth, NULL, server_thread, &server_socket);

    client_socket_class client_socket;
    client_socket.create_fd ();

    unsigned int message_length = 128;
    unsigned char p_recv_buffer[128];
    unsigned char p_big_buffer[128];
    p_big_buffer[0] = message_length - 1;
    for (int i = 1; i < 128; i ++)
        p_big_buffer[i] = i;

    client_socket.send_data (p_big_buffer, message_length);
    client_socket.recv_data (p_recv_buffer, message_length);
    client_socket.close_fd ();

    EXPECT_EQ (0, memcmp (p_big_buffer, p_recv_buffer, message_length));

    pthread_cancel (pth);
    pthread_join (pth, NULL);
    server_socket.close_client ();
    server_socket.close_fd ();
}

TEST (socket_test, two_messages_success)
{
    server_socket_class server_socket;
    server_socket.create_fd ();

    pthread_t pth;
    pthread_create (&pth, NULL, server_thread, &server_socket);

    client_socket_class client_socket;
    client_socket.create_fd ();

    unsigned char p_hello_buffer[] = { 6, 'H', 'E', 'L', 'L', 'O', 0 };
    unsigned int message_length = 7;
    unsigned char p_recv_buffer[128];

    client_socket.send_data (p_hello_buffer, message_length);
    client_socket.recv_data (p_recv_buffer, message_length);
    client_socket.close_fd ();

    EXPECT_EQ (0, memcmp (p_hello_buffer, p_recv_buffer, message_length));

    message_length = 128;
    unsigned char p_big_buffer[128];
    p_big_buffer[0] = message_length - 1;
    for (int i = 1; i < 128; i ++)
        p_big_buffer[i] = i;

    client_socket.create_fd ();
    client_socket.send_data (p_big_buffer, message_length);
    client_socket.recv_data (p_recv_buffer, message_length);
    client_socket.close_fd ();

    EXPECT_EQ (0, memcmp (p_big_buffer, p_recv_buffer, message_length));

    pthread_cancel (pth);
    pthread_join (pth, NULL);
    server_socket.close_client ();
    server_socket.close_fd ();
}

TEST (socket_test, connect_timeout_negative)
{
    client_socket_class client_socket;
    error_code_t result = client_socket.create_fd ();
    EXPECT_EQ (RESULT_SOCKET_ERROR, result);
}

TEST (socket_test, recv_timeout_negative)
{
    server_socket_class server_socket;
    server_socket.create_fd ();

    pthread_t pth;
    pthread_create (&pth, NULL, deaf_server_thread, &server_socket);

    client_socket_class client_socket;
    client_socket.create_fd ();

    unsigned char p_hello_buffer[] = { 6, 'H', 'E', 'L', 'L', 'O', 0 };
    unsigned int message_length = 7;
    unsigned char p_recv_buffer[7];

    client_socket.send_data (p_hello_buffer, message_length);
    error_code_t result = client_socket.recv_data (p_recv_buffer, message_length);
    client_socket.close_fd ();

    EXPECT_EQ (RESULT_TRANSPORT_ERROR, result);

    pthread_join (pth, NULL);
    server_socket.close_client ();
    server_socket.close_fd ();
}

TEST (socket_test, send_channel_closed_negative)
{
    server_socket_class server_socket;
    server_socket.create_fd ();

    pthread_t pth;
    pthread_create (&pth, NULL, channel_closed_server_thread, &server_socket);

    client_socket_class client_socket;
    client_socket.create_fd ();

    unsigned char p_hello_buffer[] = { 6, 'H', 'E', 'L', 'L', 'O', 0 };
    unsigned int message_length = 7;

    client_socket.send_data (p_hello_buffer, 1);

    pthread_join (pth, NULL);

    error_code_t result = client_socket.send_data (p_hello_buffer, 1);
    result = client_socket.send_data (p_hello_buffer, message_length);
    client_socket.close_fd ();

    EXPECT_EQ (RESULT_TRANSPORT_ERROR, result);
}

TEST (socket_test, recv_channel_closed_negative)
{
    server_socket_class server_socket;
    server_socket.create_fd ();

    pthread_t pth;
    pthread_create (&pth, NULL, channel_closed_server_thread, &server_socket);

    client_socket_class client_socket;
    client_socket.create_fd ();

    unsigned char p_hello_buffer[] = { 6, 'H', 'E', 'L', 'L', 'O', 0 };
    unsigned int message_length = 7;
    unsigned char p_recv_buffer[7];

    client_socket.send_data (p_hello_buffer, 1);

    pthread_join (pth, NULL);

    error_code_t result = client_socket.recv_data (p_recv_buffer, message_length);
    client_socket.close_fd ();

    EXPECT_EQ (RESULT_TRANSPORT_ERROR, result);
}
