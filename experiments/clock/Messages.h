#ifndef MESSAGES_H_
#define MESSAGES_H

//New messages should be added to this header file.//Use int32_t for message members to ensure the same bit size on all machines.
#include <stdint.h>
#include <sys/types.h>

// Describes the Physical Entities in the Paradigm (i.e. Step 1, Draw Box)
enum 
{
	SENDER_CONTROLLER = 0,
	SENDER_CLOCK = 1,
	SENDER_GRIDSERVER = 2,
	SENDER_CLIENT = 3
};

// Describes what the Physical Entities can send to each other (i.e. Step 2, How Does Box Communicate With World)
enum 
{
	MSG_ACK = 99,
	MSG_ASSIGN_ID = 0,
    MSG_HEARTBEAT = 1
};

/*
//All messages should contain a static const member named 'size'.
//'size' should specify the byte size of the message, NOT including 'size' itself.
typedef struct
{
    int32_t id;
    float xpos;
    float ypos;
    static const size_t size = 12;
} Msg_example;
*/

/*
	uint16_t = 2 bytes
	int32_t = 4 bytes
	float = 4 bytes
	bool = 1 byte
	char = 1 byte
	
	
*/

// Header Message to Identify What Kind Of Message Is Being Sent and Who It Came From
typedef struct
{
	uint16_t sender;
	uint16_t message;
	static const size_t size = 4;
} Msg_header;

// Assign An ID to a Controller
// MSG_ASSIGN_ID = 0
typedef struct 
{
	uint32_t uId;
	static const size_t size = 4;
} Msg_uId;

// Message To Carry A Heartbeat
// MSG_HEARTBEAT = 1
typedef struct
{
    uint16_t hb;
    static const size_t size = 2;
} Msg_HB;

// 
// MSG_ = 2

// Tells A GridServer to Startup on a Port, with 
// MSG_ = 3

// 
// MSG_ = 4

// 
// MSG_ = 5

// 
// MSG_ = 6

// 
// MSG_ = 7

// 
// MSG_ = 8

// 
// MSG_ = 9


#endif
