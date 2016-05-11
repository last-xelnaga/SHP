
//#include <stdlib.h>
//#include <unistd.h>
//#include <stdio.h>

#include "queue_manager.hpp"
#include "message.hpp"
#include "socket.hpp"


queue_manager_class *queue_manager_class::p_instance = 0;

queue_manager_class::queue_manager_class (
        void)
{

}

void queue_manager_class::set_socket (
        socket_class* p_socket)
{

}

int queue_manager_class::add_message (
        message_class* p_message)
{
    return 0;
}

int queue_manager_class::run (
        void)
{
    return 0;
}
