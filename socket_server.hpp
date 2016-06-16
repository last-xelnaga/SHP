
#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include "socket.hpp"


class message_class;

class server_socket_class
{
private:
    int socket_fd;
    int accept_fd;

    unsigned int write_timeout;
    unsigned int read_timeout;

public:
    server_socket_class (
            void);

    error_code_t bind (
            unsigned int port_number);

    error_code_t accept_client (
            void);

    error_code_t recv_message (
            message_class** p_message);

    error_code_t send_message (
            message_class* p_message);

    void close_client (
            void);

    void close (
            void);

    ~server_socket_class (
            void);
};

#endif // SOCKET_SERVER_HPP
