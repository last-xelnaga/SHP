
#include "debug.hpp"
#include "message.hpp"
#include "socket_client.hpp"
#include "zlib.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK           16384
#define NEW_BINARY      "new_binary"


error_code_t write_to_file (
        unsigned char* p_buffer,
        unsigned int buffer_size,
        FILE *dest)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (fwrite (p_buffer, 1, buffer_size, dest) != buffer_size)
    {
        LOG_MESSAGE ("fwrite failed %s", strerror (errno));
        result = RESULT_SYSTEM_ERROR;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t decompress (
        unsigned char* source,
        unsigned int source_size,
        FILE *dest)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // allocate inflate state
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = source_size;
    strm.next_in = source;

    // and initialize it
    if (result == RESULT_OK)
    {
        int ret = inflateInit (&strm);
        if (ret != Z_OK)
        {
            LOG_MESSAGE ("inflateInit failed. %d", ret);
            result = RESULT_SYSTEM_ERROR;
        }
    }

    // run inflate() on input until output buffer not full
    if (result == RESULT_OK)
    {
        unsigned char p_buffer [CHUNK];

        do {
            strm.avail_out = CHUNK;
            strm.next_out = p_buffer;
            int ret = inflate (&strm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END)
            {
                LOG_MESSAGE ("inflate failed. %d", ret);
                result = RESULT_SYSTEM_ERROR;
            }
            else
            {
                result = write_to_file (p_buffer, CHUNK - strm.avail_out, dest);
            }
        } while (strm.avail_out == 0 && result == RESULT_OK);
    }

    // clean up and return
    inflateEnd (&strm);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t process_update (
        unsigned int data_size,
        unsigned char* p_data)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // save data
    if (result == RESULT_OK)
    {
        DEBUG_LOG_MESSAGE ("data_size %d", data_size);

        FILE *fptr = fopen (NEW_BINARY, "w");
        result = decompress (p_data, data_size, fptr);
        fclose (fptr);
    }

    if (result == RESULT_OK)
    {
        LOG_MESSAGE ("new version. restart application...");
        sleep (3);
        exit (1);
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t process_version (
        client_socket_class* const p_socket,
        unsigned int revision)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // populate message
    message_class message (message_class::send_version);
    if (result == RESULT_OK)
    {
        DEBUG_LOG_MESSAGE ("current revision is %d", revision);
        result = message.add_num_to_message (message_class::version, revision);
    }

    // connect or reconnect if needed
    if (result == RESULT_OK)
    {
        result = p_socket->connect (/*config ()->network.address.c_str ()*/"127.0.0.1",
                /*config ()->network.port*/3456);
    }

    // send message and wait for answer
    message_class* p_answer = NULL;
    if (result == RESULT_OK)
    {
        result = p_socket->send_and_receive (&message, &p_answer);
    }

    // close connection
    p_socket->close ();

    // check the message id
    if (result == RESULT_OK)
    {
        if (message_class::send_version_result != p_answer->get_message_id ())
        {
            LOG_MESSAGE ("wrong message id. waited for %d, but got %d",
                    message_class::send_version_result, p_answer->get_message_id ());
            result = RESULT_INVALID_STATE;
        }
    }

    // acquire data
    unsigned int data_size = 0;
    unsigned char* p_data = NULL;
    if (result == RESULT_OK)
    {
        result = p_answer->get_field_from_message (message_class::update_data,
                &data_size, &p_data);

        if (result != RESULT_OK)
        {
            LOG_MESSAGE ("message does not have update_data. update not needed");
            result = RESULT_OK;
        }
        else
        {
            result = process_update (data_size, p_data);
        }
    }

    delete p_answer;

    DEBUG_LOG_TRACE_END (result)
    return result;
}
