
#include "proxy_main.hpp"
#include "debug.hpp"
#include "message.hpp"
#include "socket_server.hpp"
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>


static unsigned int get_update_file (
        const unsigned int version)
{
    unsigned int highest_version = version;
    struct dirent* dir;
    DIR* d = opendir (".");

    if (d != NULL)
    {
        while ((dir = readdir (d)) != NULL)
        {
            if (strstr (dir->d_name, ".gz") != NULL)
            {
                printf ("%s\n", dir->d_name);
                char* file_name = strdup (dir->d_name);
                file_name [strlen (file_name) - 3] = 0;
                unsigned int file_version = atoi (file_name);
                if (file_version > highest_version)
                    highest_version = file_version;

                free (file_name);
            }
        }
        closedir (d);
    }

    return highest_version;
}

void process_message_version (
        message_class* const p_message,
        server_socket_class* const p_server_socket)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    // check the version
    unsigned int version_size = 0;
    unsigned int version = 0;
    if (result == RESULT_OK)
    {
        unsigned char* p_version = NULL;
        result = p_message->get_field_from_message (message_class::version,
                &version_size, &p_version);

        if (result != RESULT_OK)
        {
            LOG_MESSAGE ("message does not have version info");
            result = RESULT_INVALID_PARAM;
        }
        else
        {
            memcpy (&version, p_version, sizeof (version));
            DEBUG_LOG_MESSAGE ("client's revision is %d", version);
        }
    }

    message_class message (message_class::send_version_result);

    unsigned int update_version = get_update_file (version);
    if (update_version > version)
    {
        char p_file_name [32] = { 0 };
        snprintf (p_file_name, sizeof (p_file_name), "%d.gz", update_version);

        FILE* fptr = fopen (p_file_name,"r");
        fseek (fptr, 0, SEEK_END);
        unsigned int file_size = ftell (fptr);
        fseek (fptr, 0, SEEK_SET);
        unsigned char* p_buffer = new unsigned char [file_size];
        if (file_size != fread (p_buffer, 1, file_size, fptr))
        {
            DEBUG_LOG_MESSAGE ("did not read the whole file");
        }
        fclose (fptr);

        message.add_field_to_message (message_class::update_data, p_buffer, file_size);
        delete [] p_buffer;
    }

    result = p_server_socket->send_message (&message);
}
