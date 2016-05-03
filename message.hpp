#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "error_codes.hpp"

// socket protocol version
#define CURRENT_MESSAGE_VERSION     1

// size of the default message
#define INITIAL_MESSAGE_SIZE        364

// default values for the mew message
#define MESSAGE_INIT                {{CURRENT_MESSAGE_VERSION, 0, 0, 0}, NULL, 0}

// Max lengths of the fields
#define FIELD_BUFFER_10240_MAX      10240

// List of the message types. This list should be in sync with clients
typedef enum
{
    // Device Security
    MESSAGE_DEVSEC_GET_VERSION = 0,                 // 0

} message_id_t;

// List of the fields. This list should be in sync with clients
typedef enum
{
    FIELD_BUFFER_10240,         // unsigned char 10240
} field_id_t;

// header structure. every message starts with this object.
// it describes the message: version, operation and payload strim,
// that will have all needed data in TLV format.
// all data use little endian.
typedef struct
{
    unsigned char message_version;
    unsigned int message_id;
    unsigned int payload_size;
    unsigned int message_result;
}__attribute__((__packed__)) message_header_t;

// working structure that describes the whole message.
// maybe in someday it will be rewritten in c++ and all these
// functions will become methods
typedef struct
{
    message_header_t header;
    unsigned char* message_data;
    unsigned int message_alloc_size;
} message_buffer_t;

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
error_code_t init_message (
        message_buffer_t* const p_message,
        const message_id_t message_id);

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
error_code_t add_field_to_message (
        message_buffer_t* const p_message,
        const int field_id,
        const unsigned int payload_size,
        const unsigned char* p_payload);

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
error_code_t get_field_from_message (
        const message_buffer_t* p_message,
        const int field_id,
        unsigned int* payload_size,
        unsigned char** p_payload);

/*
 * Function deallocates previouslly allocated memory by init_message function.
 *
 * @param p_message            [in/out]  message
 */
void destroy_message (
        message_buffer_t* const p_message);

#endif // SECD_MESSAGE_HPP
