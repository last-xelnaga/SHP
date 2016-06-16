
#include "message.hpp"
#include "debug.hpp"

#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifndef MAX
  #define MAX(a, b)               ((a) > (b) ? (a) : (b))
#endif // MAX


message_class::message_class (
        message_id_t type)
{
    DEBUG_LOG_TRACE_BEGIN

    header.message_version = CURRENT_MESSAGE_VERSION;
    header.message_id = type;
    header.payload_size = 0;
    header.message_result = RESULT_OK;

    p_raw_payload = NULL;
    message_alloc_size = 0;

    DEBUG_LOG_TRACE_END (RESULT_OK)
}

message_class::message_class (
        message_header_t header_)
{
    DEBUG_LOG_TRACE_BEGIN

    header.message_version = header_.message_version;
    header.message_id = header_.message_id;
    header.payload_size = 0;
    header.message_result = header_.message_result;

    if (header_.payload_size)
    {
        header.payload_size = header_.payload_size;
        p_raw_payload = new unsigned char [header_.payload_size];
        message_alloc_size = header_.payload_size;
    }
    else
    {
        p_raw_payload = NULL;
        message_alloc_size = 0;
    }

    DEBUG_LOG_TRACE_END (RESULT_OK)
}

error_code_t message_class::add_field_to_message (
        const field_id_t field_id,
        const unsigned char* p_payload,
        const unsigned int payload_size)
{
    error_code_t result = RESULT_OK;
    unsigned int full_payload_size = sizeof (field_id_t) +
            sizeof (unsigned int) + payload_size;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        if (header.payload_size + full_payload_size > message_alloc_size)
        {
            unsigned int new_message_alloc_size =
                    MAX ((message_alloc_size * 2),
                    (header.payload_size + full_payload_size));

            unsigned char* p_new_payload = new unsigned char [new_message_alloc_size];
            if (p_new_payload == NULL)
            {
                DEBUG_LOG_MESSAGE ("failed to allocate memory block");
                result = RESULT_NOT_ENOUGH_MEMORY;
            }
            else
            {
                DEBUG_LOG_MESSAGE ("new_message_alloc_size %d", new_message_alloc_size);

                if (p_raw_payload != NULL)
                {
                    memcpy (p_new_payload, p_raw_payload, header.payload_size);
                    delete[] p_raw_payload;
                }

                p_raw_payload = p_new_payload;
                message_alloc_size = new_message_alloc_size;
            }
        }
    }

    if (result == RESULT_OK)
    {
        unsigned char* p_message_pointer = p_raw_payload;

        // set pointer into the end of current message
        p_message_pointer += header.payload_size;

        // tag
        memcpy (p_message_pointer, &field_id, sizeof (field_id_t));
        p_message_pointer += sizeof (field_id_t);

        // length
        memcpy (p_message_pointer, &payload_size, sizeof (unsigned int));
        p_message_pointer += sizeof (unsigned int);

        // value
        memcpy (p_message_pointer, p_payload, payload_size);

        header.payload_size += full_payload_size;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t message_class::add_time_to_message (
        void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    time_t cur_time = time (NULL);

    result = add_field_to_message (message_time, (unsigned char*) &cur_time, sizeof (time_t));

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t message_class::add_string_to_message (
        const field_id_t field_id,
        const char* p_payload)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    result = add_field_to_message (field_id, (unsigned char*) p_payload, strlen (p_payload) + 1);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t message_class::get_field_from_message (
        const field_id_t field_id,
        unsigned int* payload_size,
        unsigned char** p_payload)
{
    error_code_t result = RESULT_INVALID_PARAM;
    unsigned char* p_message_pointer = p_raw_payload;
    unsigned int size_to_parse = header.payload_size;
    DEBUG_LOG_TRACE_BEGIN

    while ((size_to_parse > 0) && (result == RESULT_INVALID_PARAM))
    {
        field_id_t current_field_id = message_time;
        memcpy (&current_field_id, p_message_pointer, sizeof (field_id_t));
        p_message_pointer += sizeof (field_id_t);

        unsigned int current_payload_size = 0;
        memcpy (&current_payload_size, p_message_pointer, sizeof (unsigned int));
        p_message_pointer += sizeof (unsigned int);

        if (field_id == current_field_id)
        {
            result = RESULT_OK;

            *payload_size = current_payload_size;
            *p_payload = (unsigned char*) p_message_pointer;
        }
        else
        {
            size_to_parse -= sizeof (field_id_t) + sizeof (unsigned int) + current_payload_size;
            p_message_pointer += current_payload_size;
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

message_class::~message_class (
        void)
{
    DEBUG_LOG_TRACE_BEGIN

    header.payload_size = 0;

    if (p_raw_payload != NULL)
        delete[] p_raw_payload;

    p_raw_payload = NULL;
    message_alloc_size = 0;

    DEBUG_LOG_TRACE_END (RESULT_OK)
}
