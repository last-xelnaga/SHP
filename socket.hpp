
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "error_codes.hpp"

error_code_t create_socket (
        int* fd);

error_code_t connect_to_server (
        const int fd,
        const char* p_server_name,
        unsigned int port_number);

error_code_t bind_on_server (
        const int fd,
        unsigned int port_number);

error_code_t set_blocking (
        const int fd,
        bool is_blocking);

error_code_t accept (
        const int fd,
        int* accept_fd);

error_code_t send_data (
        const int fd,
        unsigned char* p_buffer,
        unsigned int total_to_send,
        unsigned int timeout);

error_code_t recv_data (
        const int fd,
        unsigned char* p_buffer,
        unsigned int to_receive,
        unsigned int timeout);

void close_socket (
        const int fd);

#endif // SOCKET_HPP
