
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
#include "error_codes.hpp"

class socket_class
{
protected:
    int socket_fd;
    struct sockaddr_in socket_address;
    unsigned int port_number;
    char p_server_name[256];
    unsigned int connect_retry_sleep;
    unsigned int try_count_max;
    unsigned int write_timeout; 
    unsigned int read_timeout;

    socket_class (
            void);

    error_code_t send_data (
            int fd,
            unsigned char* p_buffer,
            unsigned int total_to_send);

    error_code_t recv_data (
            int fd,
            unsigned char* p_buffer,
            unsigned int to_receive);

public:
    virtual error_code_t create_fd (
            void) = 0;

    void close_fd (
            void);
};

class client_socket_class : public socket_class
{

public:
    client_socket_class (
            void) {};

    client_socket_class (
            const char* p_server_name,
            unsigned int port);

    error_code_t create_fd (
            void);

    error_code_t send_data (
            unsigned char* p_buffer,
            unsigned int total_to_send);

    error_code_t recv_data (
            unsigned char* p_buffer,
            unsigned int to_receive);
};

class server_socket_class : public socket_class
{
private:
    int accept_fd;

public:
    server_socket_class (
            void);

    error_code_t create_fd (
            void);

    error_code_t accept_client (
            void);

    error_code_t send_data (
            unsigned char* p_buffer,
            unsigned int total_to_send);

    error_code_t recv_data (
            unsigned char* p_buffer,
            unsigned int to_receive);

    void close_client (
            void);
};

#endif // SOCKET_HPP
