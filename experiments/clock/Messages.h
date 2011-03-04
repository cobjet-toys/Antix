#ifndef MESSAGES_H_
#define MESSAGES_H

//New messages should be added to this header file.//Use int32_t for message members to ensure the same bit size on all machines.
//All messages should contain a static const member named 'size'.
//'size' should specify the byte size of the message, NOT including 'size' itself.
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
    MSG_HEARTBEAT = 1,
	MSG_INITTEAM = 2,
	MSG_INITPUCKS = 3,
	MSG_SPECIFYGRIDINFO = 4,
	MSG_ROBOTSENSORDATA = 5,
	MSG_
};

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

// Tells A GridServer To Initialize A Team With Robots And It's Client
// Tell A Client To Connect To A GridServer And Control A Team
// MSG_INITTEAM = 2
typedef struct
{
	uint32_t IP;
	uint16_t Port;
	uint32_t uId;
	uint16_t teamID;
	uint32_t numberOfRobots;
    static const size_t size = 16;
} Msg_InitTeam;

// Tells A GridServer to Initialize Pucks
// MSG_INITPUCKS = 3
typedef struct
{
	uint32_t numberOfPucks;
    static const size_t size = 4;
} Msg_InitPucks;

// Tells The Clock About A GridServers IP, Port, and GridServerID(uID)
// Tells A GridServer About It's GridServer Neighbours IP, Port, and GridServerID(uID)
// Tells The Drawer About A GridServer's IP, Port, and GridServerID(uID)
// Tells A GridServer About The Drawer's IP, Port, and ID(uID)
// MSG_SPECIFYGRIDINFO = 4
typedef struct
{
	uint32_t IP;
	uint16_t Port;
	uint32_t uId;
    static const size_t size = 10;
} Msg_SpecifyGridInfo;

// GridServer Sends Client RobotSensorData
// MSG_RobotSensorData = 5
typedef struct
{
	uint32_t id; 		//Sensor Data For This Robot, Identified By id
	float xPosition;	
	float yPosition;	
	float speed;		
	float rotSpeed;		
	uint16_t teamID;	//Team Of Robot Detected
	bool hasPuck;
	static const size_t size = 23;
} Msg_RobotSensorData;

// GridServer Sends Client RobotSensorData
// MSG_PuckSensorData = 6
typedef struct
{
	uint32_t id; 		//Sensor Data For This Robot, Identified By id
	float xPosition;	
	float yPosition;	
	static const size_t size = 12;
} Msg_RobotSensorData;

// Client Robot Makes An Action Request To A GridServer
// MSG_ = 7

// Tell Neighbor GridServer A Robot is Entering
// Tell Client Robot Is Now On New Grid Server
// MSG_ = 8

// Tell Drawer About Robot
// MSG_ = 9

// Tell Drawer About Puck
// MSG_ = 10

// Tell Drawer About Initialization Details
// MSG_ = 11


#endif
