#ifndef PROTOCOL_H
#define PROTOCOL_H

#define VERSION                 1
#define PROBE_NAME_LEN          32
#define SENSOR_NAME_LEN         64
#define MESSAGE_LEN             32
#define EVENT_PAYLOAD_LEN       32

// probe initialization
struct probe_to_server_init_t
{
    // header
    unsigned short  message_size;
    unsigned char   message_id;
    unsigned char   message_version;

    // probe description
    unsigned char   probe_id;
    unsigned int    probe_time;
    unsigned char   probe_name_len;
    char            probe_name [PROBE_NAME_LEN];
};

struct server_to_probe_init_t
{
    // header
    unsigned short  message_size;
    unsigned char   message_id;
    unsigned char   message_version;

    // result and error message
    unsigned int    result;
    unsigned char   msg_length;
    char            msg [MESSAGE_LEN];
};



struct probe_to_server_sensor_t
{
    // header
    unsigned short  message_size;
    unsigned char   message_id;
    unsigned char   message_version;

    // probe description
    unsigned char   probe_id;

    // sensor description
    unsigned char   sensor_id;
    unsigned char   sensor_type;
    unsigned char   sensor_name_len;
    char            sensor_name [SENSOR_NAME_LEN];

    // latest event
    unsigned int    event_time;
    unsigned char   event_payload [EVENT_PAYLOAD_LEN];
};

struct server_to_probe_sensor_t
{
    // header
    unsigned short  message_size;
    unsigned char   message_id;
    unsigned char   message_version;

    // result and error message
    unsigned int    result;
    unsigned char   msg_length;
    char            msg [MESSAGE_LEN];
};




struct probe_to_server_event_t
{
    // header
    unsigned short version;
    //unsigned short size;
    int event_time;

    // probe
    unsigned char probe_id;

    // sensors
    unsigned char sensor_id;
    unsigned char payload [32];
};


struct server_to_probe_event_t
{
    // header
    unsigned short version;
    //unsigned short size;

    // result and error message
    unsigned int result;
    unsigned char msg_length;
    char msg [32];
};




struct server_to_probe_command_t
{
    // header
    unsigned short version;
    //unsigned short size;

    // sensor
    unsigned char id;
    unsigned char command [32];
};

struct probe_to_server_command_t
{
    // header
    unsigned short version;
    //unsigned short size;

    // result and error message
    unsigned int result;
    unsigned char msg_length;
    char msg [32];
};




struct client_to_server_init_t
{
    // header
    unsigned short version;
    //unsigned short size;

    // client description
    unsigned char id;
    unsigned char name_length;
    char name [32];
};

\
struct probe_t
{
    // probe description
    unsigned char id;
    int probe_time;
    unsigned char name_length;
    char name [32];

    // sensors
    unsigned char count;
    struct sensor_t* sensors;
};

struct server_to_client_init_t
{
    // header
    unsigned short version;
    unsigned short size;

    // sensors
    unsigned char count;
    struct sensor_t* sensors;
};



struct server_to_client_event_t
{
    // header
    unsigned short version;

    int event_time;

    // sensors
    unsigned char sensor_id;
    unsigned char payload [32];

};

struct client_to_server_event_t
{
    // header
    unsigned short version;
    //unsigned short size;

    // result and error message
    unsigned int result;
    unsigned char msg_length;
    char msg [32];

};



struct client_to_server_command_t
{
    // header
    unsigned short version;

    // sensors
    unsigned char sensor_id;
    unsigned char payload [32];

};

struct server_to_client_command_t
{
    // header
    unsigned short version;
    //unsigned short size;

    // result and error message
    unsigned int result;
    unsigned char msg_length;
    char msg [32];


};




#endif // PROTOCOL_H
