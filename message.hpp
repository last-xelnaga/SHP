
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "error_codes.hpp"



// size of the default message
//#define INITIAL_MESSAGE_SIZE        364

// default values for the mew message
//#define MESSAGE_INIT                {{CURRENT_MESSAGE_VERSION, 0, 0, 0}, NULL, 0}

// Max lengths of the fields
//#define FIELD_BUFFER_10240_MAX      10240

// List of the message types. This list should be in sync with clients
/*typedef enum
{
    // Device Security
    MESSAGE_DEVSEC_GET_VERSION = 0,                 // 0

} message_id_t;*/

// List of the fields. This list should be in sync with clients
/*typedef enum
{
    FIELD_BUFFER_10240,         // unsigned char 10240
} field_id_t;*/

/*
 * Function initializes an empty message. Should be called before any operation
 * with message, e.g. add/get and destroy. Functiona will allocate memory, so
 * it is caller's responcibility to call destroy_message() to deallocate it.
 *
 * @param p_message            [in/out]  message
 * @param message_id           [in]  message number
 *
 * @retval TCredMgr_Result error code
 */
/*error_code_t init_message (
        message_buffer_t* const p_message,
        const message_id_t message_id);*/

/*
 * Function adds a field to the message. Message will be stored in TLV format.
 * In case of huge size of the payload the message buffer will be reallocated.
 *
 * @param p_message            [in/out]  message
 * @param field_id             [in]  field number
 * @param payload_size         [in]  data size that will be added
 * @param p_payload            [in]  data
 *
 * @retval error_code_t        error code
 */
/*error_code_t add_field_to_message (
        message_buffer_t* const p_message,
        const int field_id,
        const unsigned int payload_size,
        const unsigned char* p_payload);*/

/*
 * Function gets a field from the message.
 *
 * @param p_message            [in]  message
 * @param field_id             [in]  field number
 * @param payload_size         [out] data size
 * @param p_payload            [out] pointer to the raw data
 *
 * @retval error_code_t        error code
 */
/*error_code_t get_field_from_message (
        const message_buffer_t* p_message,
        const int field_id,
        unsigned int* payload_size,
        unsigned char** p_payload);*/

/*
 * Function deallocates previouslly allocated memory by init_message function.
 *
 * @param p_message            [in/out]  message
 */

/*void destroy_message (
        message_buffer_t* const p_message);*/

typedef struct
{
    unsigned char message_version;
    unsigned int message_id;
    unsigned int payload_size;
    unsigned int message_result;
}__attribute__((__packed__)) message_header_t;

class message_class
{
public:
    // list of the message types
    typedef enum
    {
        // system messages
        send_handshake = 0,
        get_version,
        get_configuration,

        send_temp = 100,
        send_humid,
        send_reed,

        trigger_relay = 200,
        trigger_servo,
        trigger_button,

    } message_id_t;

private:
    message_header_t header;

protected:
    unsigned char* p_raw_payload;

protected:
    void update_header (
            unsigned int payload_length)
    {
        header.payload_size = payload_length;
    }

    virtual void pack_payload (
            void) = 0;

    virtual void unpack_payload (
            void) = 0;

public:
    message_class (
            message_id_t type);

    void set_raw_message (
            unsigned char* p_raw_message,
            unsigned int raw_message_length);

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

    virtual ~message_class (
            void);
};

#endif // MESSAGE_HPP
