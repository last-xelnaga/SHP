#ifndef MESSAGES_H
#define MESSAGES_H

#define VERSION       1


struct sensor_t
{
    unsigned char id;
    unsigned char type;
    unsigned char name_length;
    char name [32];
    unsigned char payload [32];
};

struct probe_to_server_init_t
{
    // header
    unsigned short version;
    unsigned short size;

    // probe description
    unsigned char id;
    int probe_time;
    unsigned char name_length;
    char name [32];

    // sensors
    unsigned char count;
    struct sensor_t* sensors;
};

struct server_to_probe_init_t
{
    // header
    unsigned short version;
    //unsigned short size;

    // result and error message
    unsigned int result;
    unsigned char msg_length;
    char msg [32];
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




#endif // MESSAGES_H
