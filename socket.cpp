#include "debug.hpp"
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "socket.hpp"
//#include "secd_memory_debug.hpp"


socket_class::socket_class (
        void)
{
    socket_fd = -1;
    memset (&socket_address, 0, sizeof (sockaddr_in));
    port_number = 3456;
    strcpy (p_server_name, "127.0.0.1");
    connect_retry_sleep = 2;
    try_count_max = 3;
    write_timeout = 10;
    read_timeout = 10;
}

error_code_t socket_class::send_data (
        int fd,
        unsigned char* p_buffer,
        unsigned int total_to_send)
{
    error_code_t result = RESULT_OK;
    struct timeval tv;
    fd_set writeset, tempset;
    DEBUG_LOG_TRACE_BEGIN

    // Initialize the set
    FD_ZERO (&writeset);
    FD_SET (fd, &writeset);

    // Initialize time out struct
    tv.tv_sec = 0;
    tv.tv_usec = write_timeout * 1000;

    do
    {
        memcpy (&tempset, &writeset, sizeof (writeset));

        int res = select (fd + 1, NULL, &tempset, NULL, &tv);
        if (res > 0)
        {
            int sent = send (fd, p_buffer, total_to_send, 0);
            if (sent < 0)
            {
                DEBUG_LOG_MESSAGE ("send failed: %s", strerror (errno));
                result = RESULT_TRANSPORT_ERROR;
            }
            else if (sent > 0)
            {
                total_to_send -= sent;
                p_buffer += sent;
            }
        }
        else if (res == 0)
        {
            // time out
            DEBUG_LOG_MESSAGE ("send failed. timeout");
            result = RESULT_TRANSPORT_ERROR;
        }
        else
        {
            // error
            DEBUG_LOG_MESSAGE ("send failed. select error: %s", strerror (errno));
            result = RESULT_SYSTEM_ERROR;
        }
    } while ((result == RESULT_OK) && (total_to_send > 0));

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t socket_class::recv_data (
        int fd,
        unsigned char* p_buffer,
        unsigned int to_receive)
{
    error_code_t result = RESULT_OK;
    struct timeval tv;
    fd_set readset, tempset;
    unsigned int total_received = 0;
    unsigned char* p_recv_buffer = (unsigned char*)p_buffer;
    DEBUG_LOG_TRACE_BEGIN

    // Initialize the set
    FD_ZERO (&readset);
    FD_SET (fd, &readset);

    // Initialize time out struct
    tv.tv_sec = 0;
    tv.tv_usec = read_timeout * 1000;

    do
    {
        memcpy (&tempset, &readset, sizeof (readset));

        int res = select (fd + 1, &tempset, NULL, NULL, &tv);
        if (res > 0)
        {
            int received = recv (fd, (void*)p_recv_buffer, to_receive - total_received, 0);
            if (received < 0)
            {
                DEBUG_LOG_MESSAGE ("recv failed: %s", strerror (errno));
                result = RESULT_TRANSPORT_ERROR;
            }

            if (result == RESULT_OK)
            {
                total_received += received;
                p_recv_buffer += received;
            }
        }
        else if (res == 0)
        {
            // time out
            DEBUG_LOG_MESSAGE ("recv failed. timeout");
            result = RESULT_TRANSPORT_ERROR;
        }
        else
        {
            // error
            DEBUG_LOG_MESSAGE ("recv failed. select error: %s", strerror (errno));
            result = RESULT_SYSTEM_ERROR;
        }
    } while ((result == RESULT_OK) && (total_received < to_receive));

    DEBUG_LOG_TRACE_END (result)
    return result;
}


void socket_class::close_fd (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    if (socket_fd >= 0)
    {
        close (socket_fd);
        socket_fd = -1;
    }

    DEBUG_LOG_TRACE_END (RESULT_OK)
}

client_socket_class::client_socket_class (
        const char* p_server,
        unsigned int port)
{
    strcpy (p_server_name, p_server);
    port_number = port;
}

error_code_t client_socket_class::create_fd (
    void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    socket_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (socket_fd < 0)
    {
        DEBUG_LOG_MESSAGE ("socket call failed: %s", strerror(errno));
        result = RESULT_SYSTEM_ERROR;
    }

    // Get system information
    if (result == 0)
    {
        struct hostent* hPtr = gethostbyname (p_server_name);
        if (hPtr == NULL)
        {
            DEBUG_LOG_MESSAGE ("gethostbyname failed: %s", strerror(errno));
            result = RESULT_SOCKET_ERROR;
        }
        else
        {
            memcpy ((char*)&socket_address.sin_addr, hPtr->h_addr, hPtr->h_length);
            socket_address.sin_family = AF_INET;
            socket_address.sin_port = htons ((u_short)port_number);
        }
    }

    // Load system information for remote socket server into socket data structures
    if (result == RESULT_OK)
    {
        int res = 0;
        unsigned int try_count = 0;

        do
        {
            if (try_count)
            {
                sleep (connect_retry_sleep);
            }

            res = connect (socket_fd, (struct sockaddr*)&socket_address, sizeof(sockaddr_in));
            if (res < 0)
            {
                DEBUG_LOG_MESSAGE ("connect call failed: %s. try once again", strerror(errno));
                try_count ++;
            }
        } while (res < 0 && try_count < try_count_max);

        if (try_count >= try_count_max)
        {
            // no attempts left
            DEBUG_LOG_MESSAGE ("connect call failed: %s. no attempts left", strerror(errno));
            result = RESULT_SOCKET_ERROR;
        }
    }

    // set non-blocking mode
    if (result == RESULT_OK)
    {
        int res = fcntl (socket_fd, F_GETFL, 0);
        if (res != -1)
            fcntl (socket_fd, F_SETFL, res | O_NONBLOCK);
    }
    else
    {
        close_fd();
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t client_socket_class::send_data (
        unsigned char* p_buffer,
        unsigned int total_to_send)
{
    return socket_class::send_data (socket_fd, p_buffer, total_to_send);
}

error_code_t client_socket_class::recv_data (
        unsigned char* p_buffer,
        unsigned int to_receive)
{
    return socket_class::recv_data (socket_fd, p_buffer, to_receive);
}

server_socket_class::server_socket_class (
        void)
{
    accept_fd = -1;
}

error_code_t server_socket_class::create_fd (
        void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    socket_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (socket_fd < 0)
    {
        DEBUG_LOG_MESSAGE ("socket call failed: %s", strerror(errno));
        result = RESULT_SYSTEM_ERROR;
    }
    else
    {
        socket_address.sin_family = AF_INET;
        socket_address.sin_addr.s_addr = htonl (INADDR_ANY);
        socket_address.sin_port = htons (port_number);
    }

    if (result == RESULT_OK)
    {
        int yes = 1;
        int res = setsockopt (socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (yes));
        if (res < 0)
        {
            DEBUG_LOG_MESSAGE ("setsockopt failed: %s", strerror(errno));
            result = RESULT_SYSTEM_ERROR;
        }
    }

    if (result == RESULT_OK)
    {
        int res = bind (socket_fd, (struct sockaddr *) &socket_address, sizeof (struct sockaddr_in));
        if (res < 0)
        {
            DEBUG_LOG_MESSAGE ("bind failed: %s", strerror(errno));
            result = RESULT_SYSTEM_ERROR;
        }
    }

    if (result == RESULT_OK)
    {
        int res = listen (socket_fd, 1);
        if (res < 0)
        {
            DEBUG_LOG_MESSAGE ("listen failed: %s", strerror(errno));
            result = RESULT_SYSTEM_ERROR;
        }
    }

    if (result != RESULT_OK)
    {
        close_fd();
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t server_socket_class::accept_client (
        void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (socket_fd < 0)
    {
        DEBUG_LOG_MESSAGE ("socket is not ready");
        result = RESULT_INVALID_STATE;
    }

    // set blocking mode
    if (result == RESULT_OK)
    {
        int res = fcntl (socket_fd, F_GETFL, 0);
        if (res != -1)
            fcntl (socket_fd, F_SETFL, res & ~O_NONBLOCK);
    }

    if (result == RESULT_OK)
    {
        accept_fd = accept (socket_fd, NULL, NULL);
        if (accept_fd < 0)
        {
            DEBUG_LOG_MESSAGE ("accept failed: %s", strerror(errno));
            result = RESULT_SOCKET_ERROR;
        }
    }

    // set non-blocking mode
    if (result == RESULT_OK)
    {
        int res = fcntl (socket_fd, F_GETFL, 0);
        if (res != -1)
            fcntl (socket_fd, F_SETFL, res | O_NONBLOCK);
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t server_socket_class::send_data (
        unsigned char* p_buffer,
        unsigned int total_to_send)
{
    return socket_class::send_data (accept_fd, p_buffer, total_to_send);
}

error_code_t server_socket_class::recv_data (
        unsigned char* p_buffer,
        unsigned int to_receive)
{
    return socket_class::recv_data (accept_fd, p_buffer, to_receive);
}

void server_socket_class::close_client (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    if (accept_fd >= 0)
    {
        close (accept_fd);
        accept_fd = -1;
    }

    DEBUG_LOG_TRACE_END (RESULT_OK)
}



/*
static error_code_t send_data(
        int fd, unsigned char* p_buffer,
        unsigned int total_to_send)
{
    error_code_t result = SEC_DAEMON_RESULT_OK;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    do
    {
        int sent = send(fd, p_buffer, total_to_send, 0);
        if (sent < 0)
        {
            SECD_DEBUG_LOG_ERROR("send failed: %s", strerror(errno));
            result = SEC_DAEMON_SOCKET_ERROR;
        }

        if (result == SEC_DAEMON_RESULT_OK)
        {
            total_to_send -= sent;
            p_buffer += sent;
        }
    } while ((result == SEC_DAEMON_RESULT_OK) && (total_to_send > 0));

    SECD_DEBUG_LOG_TRACE_END(result);
    return result;
}

static error_code_t recv_data(
        int fd, unsigned char* p_buffer,
        unsigned int to_receive)
{
    error_code_t result = SEC_DAEMON_RESULT_OK;
    unsigned int total_received = 0;
    unsigned char* p_recv_buffer = (unsigned char*)p_buffer;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    do
    {
        int received = recv(fd, (void*)p_recv_buffer, to_receive - total_received, 0);
        if (received < 0)
        {
            SECD_DEBUG_LOG_ERROR("recv failed: %s", strerror(errno));
            result = SEC_DAEMON_SOCKET_ERROR;
        }

        if (result == SEC_DAEMON_RESULT_OK)
        {
            total_received += received;
            p_recv_buffer += received;
        }
    } while ((result == SEC_DAEMON_RESULT_OK) && (total_received < to_receive));

    SECD_DEBUG_LOG_TRACE_END(result);
    return result;
}

static int create_client_socket(
        const char* const p_socket_name)
{
    int socket_fd = -1;
    error_code_t result = SEC_DAEMON_RESULT_OK;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    socket_fd = socket_local_client(p_socket_name,
            ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
    if (socket_fd < 0)
    {
        SECD_DEBUG_LOG_ERROR("socket_local_client failed: %s", strerror(errno));
        result = SEC_DAEMON_SOCKET_ERROR;
    }

    SECD_DEBUG_LOG_TRACE_END(result);
    return socket_fd;
}

static int accept_socket(
        int fd)
{
    int accept_fd = -1;
    error_code_t result = SEC_DAEMON_RESULT_OK;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    accept_fd = accept(fd, NULL, NULL);
    if (accept_fd == -1)
    {
        result = SEC_DAEMON_SOCKET_ERROR;
        SECD_DEBUG_LOG_ERROR("accept failed: %s", strerror(errno));
    }

    SECD_DEBUG_LOG_TRACE_END(result);
    return accept_fd;
}

static void close_socket(
        int fd)
{
    SECD_DEBUG_LOG_TRACE_BEGIN();

    if (fd >= 0)
    {
        close(fd);
    }

    SECD_DEBUG_LOG_TRACE_END(SEC_DAEMON_RESULT_OK);
}

int server_create_socket(
        const char *p_socket_name)
{
    int socket_fd = -1;
    error_code_t result = SEC_DAEMON_RESULT_OK;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    socket_fd = android_get_control_socket(p_socket_name);
    if (socket_fd == -1)
    {
        result = SEC_DAEMON_SOCKET_ERROR;
        SECD_DEBUG_LOG_ERROR("android_get_control_socket failed: %s", strerror(errno));
    }

    if (result == SEC_DAEMON_RESULT_OK)
    {
        if (listen(socket_fd, CONNECTION_QUEUE_MAX) == -1)
        {
            result = SEC_DAEMON_SOCKET_ERROR;
            close(socket_fd);
            SECD_DEBUG_LOG_ERROR("listen failed: %s", strerror(errno));
        }
    }

    SECD_DEBUG_LOG_TRACE_END(result);
    return socket_fd;
}

error_code_t server_accept_and_receive(
        message_buffer_t* const p_message,
        const int listen_fd,
        int* const accept_fd)
{
    error_code_t result = SEC_DAEMON_RESULT_OK;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    if (result == SEC_DAEMON_RESULT_OK)
    {
        *accept_fd = accept_socket(listen_fd);
        if (*accept_fd == -1)
        {
            SECD_DEBUG_LOG_ERROR("accept_socket failed");
            result = SEC_DAEMON_SOCKET_ERROR;
        }
    }

    // and start to read data from socket
    if (result == SEC_DAEMON_RESULT_OK)
    {
        result = recv_data(*accept_fd, (unsigned char*)&p_message->header,
                sizeof(message_header_t));
    }

    // check header version
    if (result == SEC_DAEMON_RESULT_OK)
    {
        if (p_message->header.message_version != CURRENT_MESSAGE_VERSION)
        {
            SECD_DEBUG_LOG_ERROR("incorrect message version. current %d, "
                    "but received %d", CURRENT_MESSAGE_VERSION,
                    p_message->header.message_version);
            result = SEC_DAEMON_TRANSPORT_ERROR;
        }
    }

    // adjust size of message
    if (result == SEC_DAEMON_RESULT_OK)
    {
        if (p_message->header.payload_size > p_message->message_alloc_size)
        {
            p_message->message_data = (unsigned char*)realloc_debug(
                    p_message->message_data, p_message->header.payload_size);
            if (p_message->message_data == NULL)
            {
                SECD_DEBUG_LOG_ERROR("failed to allocate memory block");
                result = SEC_DAEMON_NOT_ENOUGH_MEMORY;
            }
            else
            {
                p_message->message_alloc_size = p_message->header.payload_size;
                SECD_DEBUG_LOG_INFO("new_message_alloc_size %d",
                        p_message->header.payload_size);
            }
        }
    }

    if ((result == SEC_DAEMON_RESULT_OK) && (p_message->header.payload_size > 0))
    {
        result = recv_data(*accept_fd, p_message->message_data,
                p_message->header.payload_size);
    }

    SECD_DEBUG_LOG_ERROR_END(result);
    return result;
}

error_code_t server_send(
        message_buffer_t* const p_message,
        const int accept_fd)
{
    error_code_t result = SEC_DAEMON_RESULT_OK;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    if (result == SEC_DAEMON_RESULT_OK)
    {
        result = send_data(accept_fd, (unsigned char*)&p_message->header,
                sizeof(message_header_t));
    }

    if ((result == SEC_DAEMON_RESULT_OK) && (p_message->header.payload_size > 0))
    {
        result = send_data(accept_fd, p_message->message_data,
                p_message->header.payload_size);
    }

    SECD_DEBUG_LOG_ERROR_END(result);
    return result;
}

error_code_t server_close_socket(
        const int accept_fd)
{
    error_code_t result = SEC_DAEMON_RESULT_OK;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    close_socket(accept_fd);

    SECD_DEBUG_LOG_ERROR_END(result);
    return result;
}

error_code_t client_send_and_receive(
        message_buffer_t* const p_message,
        const char* const p_socket_name)
{
    int fd = -1;
    error_code_t result = SEC_DAEMON_RESULT_OK;
    SECD_DEBUG_LOG_TRACE_BEGIN();

    if (result == SEC_DAEMON_RESULT_OK)
    {
        fd = create_client_socket(p_socket_name);
        if (fd < 0)
        {
            SECD_DEBUG_LOG_ERROR("create_client_socket failed");
            result = SEC_DAEMON_SOCKET_ERROR;
        }
    }

    if (result == SEC_DAEMON_RESULT_OK)
    {
        result = send_data(fd, (unsigned char*)&p_message->header,
                sizeof(message_header_t));
    }

    if ((result == SEC_DAEMON_RESULT_OK) && (p_message->header.payload_size > 0))
    {
        result = send_data(fd, p_message->message_data,
                p_message->header.payload_size);
    }

    if (result == SEC_DAEMON_RESULT_OK)
    {
        result = recv_data(fd, (unsigned char*)&p_message->header,
                sizeof(message_header_t));
    }

    // check header version
    if (result == SEC_DAEMON_RESULT_OK)
    {
        if(p_message->header.message_version != CURRENT_MESSAGE_VERSION)
        {
            SECD_DEBUG_LOG_ERROR("incorrect message version. current %d, "
                    "but received %d", CURRENT_MESSAGE_VERSION,
                    p_message->header.message_version);
            result = SEC_DAEMON_TRANSPORT_ERROR;
        }
    }

    // adjust size of message
    if (result == SEC_DAEMON_RESULT_OK)
    {
        if (p_message->header.payload_size > p_message->message_alloc_size)
        {
            p_message->message_data = (unsigned char*)realloc_debug(
                    p_message->message_data, p_message->header.payload_size);
            if (p_message->message_data == NULL)
            {
                SECD_DEBUG_LOG_ERROR("failed to allocate memory");
                result = SEC_DAEMON_NOT_ENOUGH_MEMORY;
            }
            else
            {
                p_message->message_alloc_size = p_message->header.payload_size;
            }
        }
    }

    if ((result == SEC_DAEMON_RESULT_OK) && (p_message->header.payload_size > 0))
    {
        result = recv_data(fd, p_message->message_data,
                p_message->header.payload_size);
    }

    close_socket(fd);
    fd = -1;

    SECD_DEBUG_LOG_TRACE_END(result);
    return result;
}
*/
