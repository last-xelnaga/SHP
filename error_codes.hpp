
#ifndef ERROR_CODES_HPP
#define ERROR_CODES_HPP

// Error codes
typedef enum
{
    // all operations were successufully finished
    RESULT_OK = 0,

    //
    RESULT_SYSTEM_ERROR,

    // one of the parameters was invalid
    RESULT_INVALID_PARAM,

    RESULT_INVALID_STATE,

    // do not have valid permissions
    RESULT_SECURITY_ERROR,

    // memory handling failure
    RESULT_NOT_ENOUGH_MEMORY,

    // socket connection could not be established. client->server
    RESULT_SOCKET_ERROR,

    // connection was created, but the transport has failed.
    RESULT_TRANSPORT_ERROR,
} error_code_t;

#endif // ERROR_CODES_HPP
