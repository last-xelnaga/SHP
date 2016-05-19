
#include "message.hpp"
#include <gtest/gtest.h>


TEST (message_test, message_success)
{
    message_class* p_message = new message_class (message_class::get_version);
    p_message->add_time_to_message ();
    p_message->add_string_to_message (message_class::sensor_type, "TEST_TYPE");
    p_message->add_string_to_message (message_class::sensor_name, "TEST_SENSOR");
    p_message->add_string_to_message (message_class::sensor_data, "TEST_DATA");

    unsigned int payload_size = 0;
    unsigned char* p_payload = NULL;
    p_message->get_field_from_message (message_class::message_time, &payload_size, &p_payload);
    ASSERT_EQ (payload_size, sizeof (time_t));

    p_message->get_field_from_message (message_class::sensor_type, &payload_size, &p_payload);
    ASSERT_EQ (strcmp ((char*)p_payload, "TEST_TYPE"), 0);

    p_message->get_field_from_message (message_class::sensor_name, &payload_size, &p_payload);
    ASSERT_EQ (strcmp ((char*)p_payload, "TEST_SENSOR"), 0);

    p_message->get_field_from_message (message_class::sensor_data, &payload_size, &p_payload);
    ASSERT_EQ (strcmp ((char*)p_payload, "TEST_DATA"), 0);

    delete p_message;
}

TEST (message_test, message_wrong_field_negative)
{
    message_class* p_message = new message_class (message_class::get_version);

    unsigned int payload_size = 0;
    unsigned char* p_payload = NULL;
    error_code_t result = p_message->get_field_from_message (message_class::message_time, &payload_size, &p_payload);
    ASSERT_EQ (RESULT_INVALID_PARAM, result);

    p_message->add_string_to_message (message_class::sensor_type, "TEST_TYPE");
    result = p_message->get_field_from_message (message_class::message_time, &payload_size, &p_payload);
    ASSERT_EQ (RESULT_INVALID_PARAM, result);

    p_message->get_field_from_message (message_class::sensor_type, &payload_size, &p_payload);
    ASSERT_EQ (strcmp ((char*)p_payload, "TEST_TYPE"), 0);

    delete p_message;
}
