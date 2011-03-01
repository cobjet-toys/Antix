#ifndef MESSAGES_H_
#define MESSAGES_H

//New messages should be added to this header file.//Use int32_t for message members to ensure the same bit size on all machines.
#include <stdint.h>

enum 
{
	SENDER_CONTROLLER = 0,
	SENDER_CLOCK = 1,
	SENDER_GRIDSERVER = 2
};

enum 
{
	MSG_ASSIGN_ID = 0,
    MSG_HEARTBEAT = 1
};

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
	uint16_t sender;
	uint16_t message;
	static const uint32_t size = 4;
} Msg_header;

typedef struct 
{
	uint32_t uId;
	static const uint32_t size = 4;
} Msg_uId;

typedef struct
{
    uint16_t hb;
    static const uint16_t size = 2;
} Msg_HB;

#endif
