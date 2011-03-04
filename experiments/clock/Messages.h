#ifndef MESSAGES_H_
#define MESSAGES_H

//New messages should be added to this header file.//Use int32_t for message members to ensure the same bit size on all machines.
//All messages should contain a static const member named 'size'.
//'size' should specify the byte size of the message, NOT including 'size' itself.
#include <stdint.h>
#include <sys/types.h>
#include <arpa/inet.h> //Required for INET6_ADDRSTRLEN?

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
	MSG_INITGRIDSERVER = 3,
	MSG_SPECIFYGRIDINFO = 4,
	MSG_ROBOTSENSORDATA = 5,
	MSG_PUCKSENSORDATA = 6,
	MSG_PICKUP = 7,
	MSG_DROP = 8,
	MSG_HOLDING = 9,
	MSG_UPDATEPOSE = 10,
	MSG_SERVERBOUNDRY = 11,
	MSG_CLIENTBOUNDRY = 12,
	MSG_DRAWROBOT = 13,
	MSG_DRAWPUCK = 14,
	MSG_INITDRAWER = 15
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
	uint32_t uId;					//ID Of The Gride Your Team Is Located In
	char IP[INET6_ADDRSTRLEN];		//39 bytes
	char Port[6];					//6 bytes
	//uint32_t teamID;				//ID Of Your Team
	uint32_t numberOfRobots;
    static const size_t size = 53;
} Msg_InitTeam;

// Tells A GridServer to Initialize Pucks
// MSG_INITGRIDSERVER = 3
typedef struct
{
	uint32_t numberOfPucks;
	float FoVAngle;
	float FoVRange;
	int32_t worldSideLength;
	int16_t initialWindowSize;
    static const size_t size = 18;
} Msg_InitGridServer;

// Tells The Clock About A GridServers IP, Port, and GridServerID(uID)
// Tells A GridServer About It's GridServer Neighbours IP, Port, and GridServerID(uID)
// Tells The Drawer About A GridServer's IP, Port, and GridServerID(uID)
// Tells A GridServer About The Drawer's IP, Port, and ID(uID)
// MSG_SPECIFYGRIDINFO = 4
typedef struct
{
	uint32_t uId;
	char IP[INET6_ADDRSTRLEN];		//39 bytes
	char Port[6];					//6 bytes
    static const size_t size = 49;
} Msg_SpecifyGridInfo;

// GridServer Sends Client RobotSensorData
// MSG_ROBOTSENSORDATA = 5
typedef struct
{
	uint32_t id; 		//Sensor Data For This Robot, Identified By id
	float xPosition;	
	float yPosition;		
	//uint16_t teamID;	//Team Of Robot Detected
	bool hasPuck;
	static const size_t size = 13;
} Msg_RobotSensorData;

// GridServer Sends Client RobotSensorData
// MSG_PUCKSENSORDATA = 6
typedef struct
{
	uint32_t id; 		//Sensor Data For This Robot, Identified By id
	float xPosition;	
	float yPosition;	
	static const size_t size = 12;
} Msg_PuckSensorData;

// Client Robot Makes A Pickup Request
// MSG_PICKUP = 7
typedef struct
{
	uint32_t id; 		//Requset For This Robot, Identified By id
	//uint16_t teamID;
	static const size_t size = 4;
} Msg_Pickup;

// Client Robot Makes An Drop Request
// MSG_DROP = 8
typedef struct
{
	uint32_t id; 		//Requset For This Robot, Identified By id
	//uint16_t teamID;
	static const size_t size = 4;
} Msg_Drop;

// Client Robot Makes A Holding Inquery
// MSG_HOLDING = 9
typedef struct
{
	uint32_t id; 		//Requset For This Robot, Identified By id
	//uint16_t teamID;
	static const size_t size = 4;
} Msg_Holding;

// Client Robot Makes An Update Pose Request
// MSG_UPDATEPOSE = 10
typedef struct
{
	uint32_t id; 		//Requset For This Robot, Identified By id
	//uint16_t teamID;
	float xPosition;	//Change in xPosition from the Robots current postition to where it wishes to go
	float yPosition;	//Change in yPosition from the Robots current postition to where it wishes to go
	static const size_t size = 12;
} Msg_UpdatePose;

// Tell GridServer Robot/Puck Is Now On New Grid Server
// MSG_SERVERBOUNDRY = 11
typedef struct
{
	uint32_t id; 					//Requset For This Robot/Puck, Identified By id
	bool type; 						//type=0 -> Robot, type=1 -> Puck
	//uint16_t teamID;	
	char IP[INET6_ADDRSTRLEN];		//39 bytes Client IP
	char Port[6];					//6 bytes Client Port
	float xPosition;	
	float yPosition;
	static const size_t size = 58;
} Msg_ServerBoundry;

// Tell Client Robot/Puck Is Now On New Grid Server
// MSG_CLIENTBOUNDRY = 12
typedef struct
{
	uint32_t id; 					//Requset For This Robot, Identified By id
	uint32_t uId;					//GridServer The Robot Is Enterting
	char IP[INET6_ADDRSTRLEN];		//39 bytes GridServer IP
	char Port[6];					//6 bytes GridServer Port
	static const size_t size = 53;
} Msg_ClientBoundry;

// Tell Drawer About Robot
// MSG_DRAWROBOT = 13
typedef struct
{
	uint32_t id; 		//Robot ID
	//uint16_t teamID;
	float xPosition;	
	float yPosition;
	float heading;
	static const size_t size = 16;
} Msg_DrawRobot;

// Tell Drawer About Puck
// MSG_DRAWPUCK = 14
typedef struct
{
	int32_t id; 		//Puck ID
	float xPosition;	
	float yPosition;
	static const size_t size = 12;
} Msg_DrawPuck;

// Tell Drawer About Initialization Details
// MSG_INITDRAWER = 15
typedef struct
{
	bool enableFoV;
	float FoVAngle;
	float FoVRange;
	int32_t worldSideLength;
	int16_t initialWindowSize;
	static const size_t size = 17;
} Msg_InitDrawer;


#endif
