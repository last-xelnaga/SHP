#include <string.h>
#include <stdlib.h>
#include "message.hpp"
#include "debug.hpp"


#ifndef MAX
  #define MAX(a, b)               ((a) > (b) ? (a) : (b))
#endif // MAX

error_code_t init_message (
        message_buffer_t* const p_message,
        const message_id_t message_id)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // init
    p_message->message_data = NULL;
    p_message->message_alloc_size = 0;

    // allocate
    p_message->message_data = (unsigned char*) malloc (INITIAL_MESSAGE_SIZE);
    if (p_message->message_data == NULL)
    {
        DEBUG_LOG_MESSAGE ("init_message failed to allocate memory block");
        result = RESULT_NOT_ENOUGH_MEMORY;
    }

    if (result == RESULT_OK)
    {
        p_message->message_alloc_size = INITIAL_MESSAGE_SIZE;

        p_message->header.message_version = CURRENT_MESSAGE_VERSION;
        p_message->header.message_id = message_id;
        p_message->header.payload_size = 0;
        p_message->header.message_result = RESULT_OK;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t add_field_to_message (
        message_buffer_t* const p_message,
        const int field_id,
        const unsigned int payload_size,
        const unsigned char* p_payload)
{
    error_code_t result = RESULT_OK;
    unsigned char* p_message_pointer = p_message->message_data;
    unsigned int full_payload_size = sizeof(int) +
            sizeof (unsigned int) + payload_size;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        switch (field_id)
        {
            case FIELD_BUFFER_10240:
                if (payload_size > FIELD_BUFFER_10240_MAX)
                {
                    DEBUG_LOG_MESSAGE ("Size of the field %s should be in range [0..%ld]",
                            "FIELD_BUFFER_10240", (unsigned long) FIELD_BUFFER_10240_MAX);
                    result = RESULT_INVALID_PARAM;
                }
                break;
        }
    }

    if (result == RESULT_OK)
    {
        if (p_message->header.payload_size + full_payload_size >
                p_message->message_alloc_size)
        {
            unsigned int new_message_alloc_size =
                    MAX ((p_message->message_alloc_size * 2),
                    (p_message->header.payload_size + full_payload_size));

            p_message->message_data = (unsigned char*) realloc (p_message->message_data,
                    new_message_alloc_size);
            if (p_message->message_data == NULL)
            {
                DEBUG_LOG_MESSAGE ("failed to allocate memory block");
                result = RESULT_NOT_ENOUGH_MEMORY;
            }

            if (result == RESULT_OK)
            {
                p_message->message_alloc_size = new_message_alloc_size;
                p_message_pointer = p_message->message_data;
                DEBUG_LOG_MESSAGE ("new_message_alloc_size %d", new_message_alloc_size);
            }
        }
    }

    if (result == RESULT_OK)
    {
        // set pointer into the end of current message
        p_message_pointer += p_message->header.payload_size;

        // tag
        memcpy (p_message_pointer, &field_id, sizeof (int));
        p_message_pointer += sizeof(int);

        // length
        memcpy (p_message_pointer, &payload_size, sizeof (unsigned int));
        p_message_pointer += sizeof (unsigned int);

        // value
        memcpy (p_message_pointer, p_payload, payload_size);

        p_message->header.payload_size += sizeof(int) +
            sizeof (unsigned int) + payload_size;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t get_field_from_message (
        const message_buffer_t *p_message,
        const int field_id,
        unsigned int* payload_size,
        unsigned char** p_payload)
{
    error_code_t result = RESULT_TRANSPORT_ERROR;
    unsigned char* p_message_pointer = p_message->message_data;
    unsigned int size_to_parse = p_message->header.payload_size;
    DEBUG_LOG_TRACE_BEGIN

    while ((size_to_parse > 0) && (result == RESULT_TRANSPORT_ERROR))
    {
        int current_field_id = 0;
        unsigned int current_payload_size = 0;

        memcpy (&current_field_id, p_message_pointer, sizeof (int));
        p_message_pointer += sizeof(int);

        memcpy (&current_payload_size, p_message_pointer, sizeof (unsigned int));
        p_message_pointer += sizeof(unsigned int);

        if (field_id == current_field_id)
        {
            result = RESULT_OK;

            switch (field_id)
            {
                case FIELD_BUFFER_10240:
                    if (current_payload_size > FIELD_BUFFER_10240_MAX)
                    {
                        DEBUG_LOG_MESSAGE("Size of the field %s should be in range [0..%ld]",
                                "FIELD_BUFFER_10240", (unsigned long) FIELD_BUFFER_10240_MAX);
                        result = RESULT_INVALID_PARAM;
                    }

                    break;
            }

            if (result == RESULT_OK)
            {
                *payload_size = current_payload_size;
                *p_payload = (unsigned char*) p_message_pointer;
            }
        }
        else
        {
            size_to_parse -= sizeof (int) + sizeof (unsigned int) + current_payload_size;
            p_message_pointer += current_payload_size;
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

void destroy_message (
        message_buffer_t* const p_message)
{
    DEBUG_LOG_TRACE_BEGIN

    p_message->message_alloc_size = 0;

    free (p_message->message_data);
    p_message->message_data = NULL;

    p_message->header.payload_size = 0;

    DEBUG_LOG_TRACE_END (RESULT_OK)
}
