
#ifndef QUEUE_MANAGER_HPP
#define QUEUE_MANAGER_HPP

#include "error_codes.hpp"


class client_socket_class;
class message_class;

class queue_manager_class
{
private:
    static queue_manager_class* p_instance;

    // list of messages

    //tee_handle_map_t tee_handle_map;

    // socket
    client_socket_class* p_socket;

private:
    queue_manager_class (
            void);

    error_code_t send_message (
            message_class* p_message);

    error_code_t peek_message (
            unsigned int* message_id,
            message_class** p_message);

    error_code_t remove_message (
            unsigned int message_id);

public:

    static queue_manager_class* instance (
        void)
    {
        if (!p_instance)
            p_instance = new queue_manager_class ();
        return p_instance;
    }

    error_code_t add_message (
            message_class* p_message);

    error_code_t run (
            client_socket_class* p_client_socket);

};

#endif // QUEUE_MANAGER_HPP
