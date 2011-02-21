#ifndef MESSAGES_H_
#define MESSAGES_H

//New messages should be added to this header file.//Use int32_t for message members to ensure the same bit size on all machines.
#include <stdint.h>

//All messages should contain a static const member named 'size'.
//'size' should specify the byte size of the message, NOT including 'size' itself.
typedef struct
{
    int32_t id;
    float xpos;
    float ypos;
    static const int size = 12;
} Msg_example;

typedef struct
{
    int32_t id;
    float xpos;
    float ypos;
    static const int size = 12;
} Msg_daemon_client;

typedef struct
{
    int32_t id;
    float xpos;
    float ypos;
    static const int size = 12;
} Msg_daemon_server;

typedef struct
{
    // Message sent by daemon to controller to indicate readiness
    // Sends CPU load average for daemon machine
    float load_one;
    float load_five;
    float load_fifteen;
    static const int size = 12;
} Msg_controller_ready;

#endif
