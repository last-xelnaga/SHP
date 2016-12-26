
#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "error_codes.hpp"

// tag from DEBUG_TAG define in Makefile
#define QUOTE(name)                 #name
#define STR(macro)                  QUOTE(macro)
#define DEBUG_TAG_NAME              STR(DEBUG_TAG)


extern int debug_level;

#define LOG_MESSAGE(...)                                  \
    debug_log_print (DEBUG_TAG_NAME, get_filename(__FILE__),    \
            __LINE__, __VA_ARGS__);


// prototypes for the Debug messages
#ifdef DEBUG
#define DEBUG_LOG_MESSAGE(...)                                  \
    debug_log_print (DEBUG_TAG_NAME, get_filename(__FILE__),    \
            __LINE__, __VA_ARGS__);
#else
    #define DEBUG_LOG_MESSAGE(...)
#endif // DEBUG


// prototypes for the Trace
#ifdef TRACE
#define DEBUG_LOG_TRACE_BEGIN                                   \
    debug_log_print (DEBUG_TAG_NAME, get_filename (__FILE__),   \
            __LINE__, "%s() started.", __FUNCTION__);           \
            ++ debug_level;
#define DEBUG_LOG_TRACE_END(__res)                              \
    -- debug_level;                                             \
    debug_log_print (DEBUG_TAG_NAME, get_filename (__FILE__),   \
            __LINE__, "%s() ended. result %d - %s",             \
            __FUNCTION__, __res, get_error_msg (__res));

#else
    #define DEBUG_LOG_TRACE_BEGIN
    #define DEBUG_LOG_TRACE_END(__res)
#endif // TRACE


// prototypes for the Data
#ifdef DATA
#define DEBUG_LOG_PRINT_ARRAY(__header, __data, __length)  \
    debug_log_print_array (DEBUG_TAG_NAME, __header, __data, __length)
#else
    #define DEBUG_LOG_PRINT_ARRAY(__header, __data, __length)
#endif // DATA


const char* get_filename (
        const char* p_filename);

/*
* Function will provide a readable description of the provided error code.
*
* @param result     [in]  secd error code.

* @return           pointer to the null terminated string with a readable
*                   description of the error.
*/
const char* get_error_msg (
        const error_code_t result);

/*
* Function will store a provided string into the buffer
* for the debug messages.
*
* @param p_tag      [in]  pointer to the tag string.
* @param p_filename [in]  file name. will be used __FILE__.
* @param line       [in]  line of the code. will be used __LINE__.
* @param text       [in]  pointer to the log message.
*/
void debug_log_print (
        const char* p_tag,
        const char* p_filename,
        const int line,
        const char* text,
        ...);

/*
* Function will show data buffer with header.
*
* @param p_tag      [in]  pointer to the tag string.
* @param p_header   [in]  header message before data.
* @param p_data     [in]  data buffer that should be shown.
* @param length     [in]  length of the data buffer.
*/
void debug_log_print_array (
        const char* p_tag,
        const char* p_header,
        const unsigned char* p_data,
        const unsigned length);

#endif // DEBUG_HPP
