
#include "debug.hpp"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define MAX_ERROR_MSG_SIZE          32
#define MAX_LOG_MSG_SIZE            1024

// size of array
#define ARR_DIM(x)                  (sizeof (x) / sizeof (x[0]))

#define INTEND_SIZE                 4
#define INTEND_DEPTH                10


typedef struct
{
    error_code_t error_code;
    char error_msg [MAX_ERROR_MSG_SIZE];
} error_code_message_t;

static const error_code_message_t secd_errors_arr [] =
{
    {
        RESULT_OK, "RESULT_OK"
    },
    {
        RESULT_SYSTEM_ERROR, "SYSTEM_ERROR"
    },
    {
        RESULT_INVALID_PARAM, "INVALID_PARAM"
    },
    {
        RESULT_INVALID_STATE, "INVALID_STATE"
    },
    {
        RESULT_SECURITY_ERROR, "SECURITY_ERROR"
    },
    {
        RESULT_NOT_ENOUGH_MEMORY, "NOT_ENOUGH_MEMORY"
    },
    {
        RESULT_SOCKET_ERROR, "SOCKET_ERROR"
    },
    {
        RESULT_TRANSPORT_ERROR, "TRANSPORT_ERROR"
    },
};


int debug_level;
bool is_initialized = false;
static char p_debug_level_buffer [INTEND_DEPTH * INTEND_SIZE + 1];

static void level_debug_init (
        void)
{
    debug_level = 0;
    memset (p_debug_level_buffer, ' ', sizeof (p_debug_level_buffer));
    p_debug_level_buffer [INTEND_DEPTH * INTEND_SIZE] = 0;
    is_initialized = true;
}

const char* get_filename (
        const char* p_filename)
{
    return strrchr (p_filename, '/') ? strrchr (p_filename, '/') + 1 : p_filename;
}

const char* get_error_msg (
        const error_code_t result)
{
    char* result_msg = NULL;

    for (unsigned int i = 0; i < ARR_DIM (secd_errors_arr); ++ i)
    {
        if (result == secd_errors_arr [i].error_code)
        {
            result_msg = (char*) secd_errors_arr [i].error_msg;
            break;
        }
    }

    return result_msg;
}

static const char* get_current_time (
        void)
{
    static char result[12];
    time_t rawtime = time (NULL);
    struct tm* timeinfo = localtime (&rawtime);
    sprintf (result, "%.2d:%.2d:%.2d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return result;
}

void debug_log_print (
        const char* p_tag,
        const char* p_filename,
        const int line,
        const char* text,
        ...)
{
    char buffer [MAX_LOG_MSG_SIZE] = { 0 };

    if (is_initialized == false)
    {
        level_debug_init ();
    }

    if (debug_level > INTEND_DEPTH)
    {
        debug_level = INTEND_DEPTH;
    }

    va_list args;
    va_start (args, text);
    vsnprintf (buffer, MAX_LOG_MSG_SIZE, text, args);
    printf ("[%s] %s %25s:%-4d  %s%s\n", p_tag, get_current_time (), p_filename, line,
            &p_debug_level_buffer [sizeof (p_debug_level_buffer) - 1 - debug_level * INTEND_SIZE],
            buffer);
    va_end (args);
}

#ifdef DATA
void debug_log_print_array (
        const char* p_tag,
        const char* p_header,
        const unsigned char* p_data,
        const unsigned length)
{
    // pattern
    char p_hex_arr [] = "0123456789abcdef";

    // hex representation string
    char p_log_hex [128] = { 0 };
    unsigned int p_log_hex_len = 0;

    // ascii representation string
    char p_log_ascii [128] = { 0 };
    unsigned int p_log_ascii_len = 0;

    // write the header
    printf ("[%s] array: %s, length: %d\n", p_tag, p_header, length);

    // process all bytes
    for (unsigned int i = 0; i < length; ++ i)
    {
        // do we have a line to show?
        if (p_log_hex_len >= 30)
        {
            // ensure the null in the end
            p_log_hex [p_log_hex_len] = 0;
            p_log_hex_len = 0;

            p_log_ascii [p_log_ascii_len] = 0;
            p_log_ascii_len = 0;

            // write the line
            printf ("[%s] %s || %s\n", p_tag, p_log_hex, p_log_ascii);
        }

        // process byte and make a hex from it
        p_log_hex [p_log_hex_len ++] = p_hex_arr [(p_data [i] & 0xf0) >> 4];
        p_log_hex [p_log_hex_len ++] = p_hex_arr [p_data [i] & 0x0f];
        p_log_hex [p_log_hex_len ++] = ' ';

        // show printable characters, otherwise show a '.'
        p_log_ascii [p_log_ascii_len ++] = p_data [i] >= 0x20 ? p_data [i] : '.';
    }

    // write last piece of data
    if (p_log_hex_len > 0)
    {
        // shift the end of the string to allign the second part
        memset (&p_log_hex [p_log_hex_len], ' ', 30 - p_log_hex_len);
        p_log_hex_len = 30;

        // ensure that our strings null-terminated
        p_log_hex [p_log_hex_len] = 0;
        p_log_ascii [p_log_ascii_len] = 0;

        printf ("[%s] %s || %s\n", p_tag, p_log_hex, p_log_ascii);
    }
}

#endif // DATA
