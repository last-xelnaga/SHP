
#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "error_codes.hpp"
#include <map>

class message_class;

// resord: id, message
typedef std::map <unsigned int, message_class*> message_map_t;


class queue_class
{
private:
    // list of messages
    message_map_t message_map;

    // highest message id number
    unsigned int message_id;

public:
    queue_class (
            void);

    error_code_t add (
            message_class* const p_message);

    error_code_t peek (
            unsigned int* id,
            message_class** p_message);

    error_code_t remove (
            const unsigned int id);

    ~queue_class (
            void);
};

#endif // QUEUE_HPP
