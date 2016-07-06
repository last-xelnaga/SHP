
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "error_codes.hpp"

// message protocol version
#define CURRENT_MESSAGE_VERSION             1

class message_class
{
public:

    typedef enum
    {
        // system messages
        send_version = 0,
        send_version_result,
        send_configuration,
        send_configuration_result,

        // sensor messages
        send_event = 100,
        send_event_result,
        //send_data,
        //send_data_result,

    } message_id_t;

    // list of the field types
    typedef enum
    {
        message_time,

        version,
        update_data,

        config_ip,
        config_port,
        config_name,
        config_sensors,

        sensor_type,
        sensor_name,
        sensor_data,

    } field_id_t;

    // header structure. every message starts with this object.
    // it describes the message: version, operation and payload strimg,
    // that will have all needed data in TLV format.
    // all data use little endian.
    typedef struct
    {
        unsigned char message_version;
        message_id_t message_id;
        unsigned int payload_size;
        error_code_t message_result;
    }__attribute__((__packed__)) message_header_t;

private:
    message_header_t header;
    unsigned char* p_raw_payload;
    unsigned int message_alloc_size;

public:
    message_class (
            message_id_t type);

    message_class (
            message_header_t header_);

    /*
     * Function adds a field to the message. Message will be stored in TLV format.
     * In case of huge size of the payload the message buffer will be reallocated.
     *
     * @param field_id             [in]  field number
     * @param p_payload            [in]  data
     * @param payload_size         [in]  data size that will be added
     *
     * @retval error_code_t        error code
     */
    error_code_t add_field_to_message (
            const field_id_t field_id,
            const unsigned char* p_payload,
            const unsigned int payload_size);

    error_code_t add_time_to_message (
            void);

    error_code_t add_string_to_message (
            const field_id_t field_id,
            const char* p_payload);

    error_code_t add_num_to_message (
            const field_id_t field_id,
            const int payload);

    /*
     * Function gets a field from the message.
     *
     * @param field_id             [in]  field number
     * @param payload_size         [out] data size
     * @param p_payload            [out] pointer to the raw data
     *
     * @retval error_code_t        error code
     */
    error_code_t get_field_from_message (
            const field_id_t field_id,
            unsigned int* payload_size,
            unsigned char** p_payload);

    void get_header (
            unsigned char** p_data,
            unsigned int* data_length)
    {
        *p_data = (unsigned char*) &header;
        *data_length = (unsigned int) sizeof (message_header_t);
    }

    void get_payload (
            unsigned char** p_data,
            unsigned int* data_length)
    {
        *p_data = p_raw_payload;
        *data_length = header.payload_size;
    }

    message_id_t get_message_id (
            void)
    {
        return header.message_id;
    }

    /*
     * Function deallocates previouslly allocated memory.
     *
     */
    virtual ~message_class (
            void);
};

#endif // MESSAGE_HPP
