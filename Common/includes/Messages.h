#ifndef MESSAGES_H_
#define MESSAGES_H_

//New messages should be added to this header file.//Use int32_t for message members to ensure the same bit size on all machines.
//All messages should contain a static const member named 'size'.
//'size' should specify the byte size of the message, NOT including 'size' itself.
#include <stdint.h>
#include <sys/types.h>
#include <arpa/inet.h> //Required for INET6_ADDRSTRLEN?

#define MAX_PORTCHARACTERS 6
// Describes the Physical Entities in the Paradigm (i.e. Step 1, Draw Box)
enum 
{
	SENDER_CLOCK = 1, // used
	SENDER_GRIDSERVER = 2, // used
	SENDER_CLIENT = 3, // used
    SENDER_CONTROLLER = 4,    
    SENDER_DRAWER = 5,
};

// Describes what the Physical Entities can send to each other (i.e. Step 2, How Does Box Communicate With World)
enum 
{
    MSG_HEARTBEAT = 1, // USED
	MSG_REQUESTINITTEAM = 2, // USED
    MSG_GRIDNEIGHBOURS = 3,
    MSG_BOUNDARYUPDATE = 4,
	MSG_RESPONDINITTEAM = 24,
    MSG_REQUESTSENSORDATA = 16, // USED
	MSG_RESPONDSENSORDATA = 5, // USED
	MSG_PROCESSACTION = 17, // USED
	MSG_RESPONDPROCESSACTION = 18,
	MSG_REGISTERROBOT = 20, // USED
	MSG_RESPONDREGISTERROBOT = 21, // USED
	MSG_UNREGISTERROBOT = 22, // USED
	MSG_RESPONDUNREGISTERROBOT = 23, // USED
    MSG_GRIDDATAFULL = 31,
    MSG_GRIDDATACOMPRESS = 32,
    MSG_SETDRAWERCONFIG = 33,
    MSG_GRIDTEAMS = 40,
	MSG_GRIDCONFIRMSTARTED	= 34,
	MSG_REQUESTGRIDWAITING = 35,
	MSG_RESPONDGRIDWAITING = 36,
	MSG_RESPONDGRIDRANGE = 37,
	MSG_CLOCKPROCEED = 38,
	MSG_CONFIRMTEAM = 39
};

// Drawer enums
enum
{
	DRAWER_STOPDATA = 'F',
	DRAWER_SENDDATA = 'T',
	DRAWER_FULLDETAILS = 'F',
	DRAWER_COMPRESSEDDETAILS = 'C',
};

typedef struct
{
    uint32_t id;
    float x;
    float y;
    static const size_t size = 12;
} Msg_InitRobot;
static const char* Msg_InitRobot_format = "lff";

typedef struct
{
    uint32_t id;
    float x;
    float y;
    static const size_t size = 12;
} Msg_TeamInit;
static const char* Msg_TeamInit_format = "lff";

typedef struct
{
    uint16_t position;
    char ip[INET6_ADDRSTRLEN];
    char port[MAX_PORTCHARACTERS];
    static const size_t size = sizeof(ip) + sizeof(port) + sizeof(position);

} Msg_GridNeighbour;
static const char* Msg_gridNeighbour_format = "hss";

// Header Message to Identify What Kind Of Message Is Being Sent and Who It Came From
typedef struct
{
	uint16_t sender;
	uint16_t message;
	static const size_t size = 4;
} Msg_header;
static const char* Msg_header_format = "hh";

// Message To Carry A Heartbeat
// MSG_HEARTBEAT = 1
typedef struct
{
    uint16_t hb;
    static const size_t size = 2;
} Msg_HB;
static const char* Msg_HB_format = "h";
// Tells A GridServer To Initialize A Team With Robots And It's Client
// Tell A Client To Connect To A GridServer And Control A Team
// MSG_INITTEAM = 2

typedef struct{
	uint16_t msgSize; //THIS IS THE NUMBER OF OBJECTS WE ARE REQUESTING
	static const size_t size = 2;
} Msg_MsgSize;
static const char* Msg_MsgSize_format = "h"; 

typedef struct
{
    uint32_t id;
    static const size_t size = 4;
} Msg_RequestSensorData;
static const char* Msg_RequestSensorData_format = "l";

// GridServer Sends Client RobotSensorData
// MSG_ROBOTSENSORDATA = 5
typedef struct
{
	uint32_t id; 		//Sensor Data For This Robot, Identified By id
	float xPosition;	
	float yPosition;		
	//uint16_t teamID;	//Team Of Robot Detected
	char hasPuck;
	static const size_t size = 13;
} Msg_RobotSensorData;
static const char* Msg_RobotSensorData_format = "lffc";

// USED
typedef struct{
	uint32_t id;
	static const size_t size = 4;
} Msg_RobotId;
static const char * Msg_RobotId_format = "l";

// USED
typedef struct{
	uint32_t id;
	uint16_t objectCount;
	static const size_t size = 6;
} Msg_SensedObjectGroupHeader;
static const char * Msg_SensedObjectGroupHeader_format = "lh";


// USED
typedef struct{
    uint32_t id;       //4
    uint32_t x;             //4
    uint32_t y;             //4
	static const size_t size = 12;
} Msg_SensedObjectGroupItem;
static const char * Msg_SensedObjectGroupItem_format = "lll";

//action: SetSpeed=0, Pickup=1, Drop=2
typedef struct{ // USED
    uint32_t robotid;       //4
    uint16_t action;        //2
    float speed;            //4
    float angle;            //4
	static const size_t size = 14;
} Msg_Action;
static const char * Msg_Action_format = "lhff";

typedef struct{
	uint32_t teamId;
	const static size_t size = 4;
} Msg_TeamId;
static const char * Msg_TeamId_format = "l";

typedef struct{
    uint32_t robotid;       //4
    float x_pos;            //4
    float y_pos;            //4
    float speed;            //4
    float angle;            //4
    uint32_t puckid;        //4
    uint16_t gridid;        //2
	static const size_t size = 26;
} Msg_RobotInfo;
static const char * Msg_RobotInfo_format = "lfffflh";

typedef struct{
   	char send_data;			//'T' or 'F'
    char data_type;			//'F' or 'C'
    float tl_x;				// top-left corner x; 0 for full grid
    float tl_y;				// top-left corner y; 0 for full grid
    float br_x;				// bottom-right corner x; 0 for full grid
    float br_y;				// bottom-right corner y; 0 for full grid
	static const size_t size = 18;
} Msg_DrawerConfig;
static const char * Msg_DrawerConfig_format = "ccffff";

typedef struct{
	uint32_t id;
	static const size_t size = 4;
} Msg_GridId;
static const char * Msg_GridId_format = "l";

typedef struct{
	uint32_t from;
	uint32_t to;
	static const size_t size = 8;
} Msg_RobotIdRange;
static const char * Msg_RobotIdRange_format = "ll";

typedef struct{
	uint32_t teams;
	uint32_t robotsPerTeam;
	static const size_t size = 8;
} Msg_GridRequestIdRage;
static const char * Msg_GridRequestIdRage_format = "ll";

typedef struct{
	char status; // 'R' or 'P'
	static const size_t size = 1;
} Msg_ClockStatus;
static const char * Msg_ClockStatus_format = "c";

#endif
