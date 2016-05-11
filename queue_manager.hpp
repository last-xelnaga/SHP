
#ifndef QUEUE_MANAGER_HPP
#define QUEUE_MANAGER_HPP

//#include "sensors/sensor.hpp"

class socket_class;
class message_class;

class queue_manager_class
{
private:
    static queue_manager_class* p_instance;

    // list of messages
    // socket

private:
    queue_manager_class (
            void);

public:

    static queue_manager_class* instance (
        void)
    {
        if (!p_instance)
            p_instance = new queue_manager_class();
        return p_instance;
    }

    void set_socket (
            socket_class* p_socket);

    int add_message (
            message_class* p_message);

    int run (
            void);

};

#endif // QUEUE_MANAGER_HPP
