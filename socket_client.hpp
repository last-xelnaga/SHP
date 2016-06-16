
#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP

#include "socket.hpp"


class message_class;

class client_socket_class
{

private:
    int socket_fd;

    unsigned int connect_retry_sleep;
    unsigned int try_count_max;

    unsigned int write_timeout;
    unsigned int read_timeout;

public:
    client_socket_class (
            void);

    error_code_t connect (
            const char* p_server_name,
            unsigned int port_number);

    error_code_t send_and_receive (
            message_class* p_message,
            message_class** p_answer);

    void close (
            void);

    ~client_socket_class (
            void);
};

#endif // SOCKET_CLIENT_HPP
