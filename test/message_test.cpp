
#include <gtest/gtest.h>
#include "message.hpp"

class test_message_class : public message_class
{
public:
    char p_text[128];
    unsigned int data;

public:
    test_message_class (
        void) : message_class (message_class::send_temp)
    {
        memset (p_text, 0, sizeof (p_text));
        data = 0;
    }

    void set_new_data (
        const char* p_new_text,
        unsigned int new_data)
    {
        strcpy (p_text, p_new_text);
        data = new_data;
    }

    void pack_payload (
            void)
    {
        p_raw_payload = new unsigned char[128];
        unsigned char* p_pointer = p_raw_payload;

        // add size of the text
        int text_length = strlen (p_text) + 1;
        memcpy (p_pointer, &text_length, sizeof (int));
        p_pointer += sizeof (int);
        memcpy (p_pointer, p_text, text_length);
        p_pointer += text_length;

        // add data
        memcpy (p_pointer, &data, sizeof (int));
        p_pointer += sizeof (int);

        update_header (p_pointer - p_raw_payload);
    }

    void unpack_payload (
            void)
    {
        unsigned char* p_pointer = p_raw_payload;

        // get text
        int text_length = 0;
        memcpy (&text_length, p_pointer, sizeof (int));
        p_pointer += sizeof (int);
        memcpy (p_text, p_pointer, text_length);
        p_pointer += text_length;

        // get data
        memcpy (&data, p_pointer, sizeof (int));
        p_pointer += sizeof (int);
    }
};

TEST (message_test, message_manipulation_success)
{
    test_message_class* p_test_message = new test_message_class ();
    p_test_message->set_new_data ("test message", 1200);
    p_test_message->pack_payload ();

    unsigned char* p_data = NULL;
    unsigned int data_length = 0;
    unsigned int data_length2 = 0;
    unsigned char* p_raw_message = new unsigned char[256];
    p_test_message->get_header (&p_data, &data_length);
    memcpy (p_raw_message, p_data, data_length);
    p_test_message->get_payload (&p_data, &data_length2);
    memcpy (&p_raw_message[data_length], p_data, data_length2);
    delete p_test_message;

    p_test_message = new test_message_class ();
    p_test_message->set_raw_message (p_raw_message, data_length + data_length2);
    delete p_raw_message;

    p_test_message->unpack_payload ();

    ASSERT_EQ (p_test_message->data, 1200);
    ASSERT_EQ (strcmp ("test message", p_test_message->p_text), 0);

    delete p_test_message;
}
