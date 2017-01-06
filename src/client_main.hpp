
#ifndef CLIENT_MAIN_HPP
#define CLIENT_MAIN_HPP

#include "error_codes.hpp"

class queue_class;

error_code_t process_version (
        const unsigned int revision);

error_code_t process_configuration (
        void);

error_code_t process_event (
        queue_class* const p_queue);

#endif // CLIENT_MAIN_HPP
