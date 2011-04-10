#include "RobotClient.h"
#include "Messages.h"
#include <string.h>
#include "Config.h"
#include <map>
#include <vector>
#include "Types.h"
#include <networkCommon.h>

#ifdef DEBUG
#include <time.h>
#endif

using namespace Network;


static time_t init_sec = time(NULL);
static int Timesteps = 0;


RobotClient::RobotClient():Client(), m_ReadyGrids(0), m_ReadyActionGrids(0)
{    
	m_totalGridRequests = 0;
	m_totalGridResponses = 0;
	m_totalRobotsReceived = 0;
	
	m_robot_FOV = 0.0f;
    m_robot_Radius = 0.0f;
    m_robot_SensorRange = 0.0f;
    m_robot_PickupRange = 0.0f;
    m_robot_WorldSize = 0.0f;
    m_robot_HomeRadius = 0.0f;
    
}

void RobotClient::initRobotGame()
{
    robotGameInstance = new RobotGame(m_robot_FOV, m_robot_Radius,m_robot_SensorRange, m_robot_PickupRange, m_robot_WorldSize,m_robot_HomeRadius);
    // make a robot_info vector for testing
    //std::vector<uid>* robot_ids;
    //robotGameInstance->requestSensorData(1, robot_ids);    
}

RobotClient::~RobotClient()
{
    delete robotGameInstance;
}

int RobotClient::sendRobotRequests()
{
    //Create our request messages
    Msg_RequestSensorData l_Req;
    Msg_header l_Header;
    Msg_MsgSize l_Size;

    DEBUGPRINT("Sending request for data to grid.\n");

    //For each grid, send a message request containing all of the robot ids for that grid.
    std::vector<int>::const_iterator l_GridEnd = m_Grids.end();
    for (std::vector<int>::const_iterator it = m_Grids.begin(); it != l_GridEnd; it++)
    {
        vector<uid> l_RobotIds;
        l_RobotIds.clear();
        DEBUGPRINT("Requesting sensory info from grid id: %d\n", m_GridFdToId[(*it)]);
        robotGameInstance->requestSensorData(m_GridFdToId[(*it)], &l_RobotIds);
        
        l_Size.msgSize = l_RobotIds.size();//REPLACE WITH ACTUAL REQUEST FOR ROBOTS(l_RobotIds.size())
        DEBUGPRINT("Requesting sensory info for %zu robots\n", l_RobotIds.size());

        unsigned int l_MessageSize = (l_Size.msgSize*l_Req.size)+l_Header.size+l_Size.size;
        unsigned char* l_Buffer = new unsigned char[l_MessageSize];

        if (l_Buffer == NULL)
        {
            ERRORPRINT("Couldn't allocate memory for a buffer to store sensory info requests\n");
            return -1;
        }

        //Keep track of where in the buffer we are packing into.
        unsigned int l_CurrBuffIndex = 0; 

        //Add our header.
        l_Header.sender = SENDER_CLIENT;
        l_Header.message = MSG_REQUESTSENSORDATA;
        l_CurrBuffIndex += NetworkCommon::packHeaderMessage(l_Buffer+l_CurrBuffIndex, &l_Header);        

         //Add the number of elements we are sending
        l_CurrBuffIndex += NetworkCommon::packSizeMessage(l_Buffer+l_CurrBuffIndex, &l_Size);
        DEBUGPRINT("Requesting sensory info for %d robots\n", l_Size.msgSize);
               
        for (int i = 0; i < l_Size.msgSize;i++)
        {
            l_Req.id = l_RobotIds[i];
            l_CurrBuffIndex += NetworkCommon::packSensReqMessage(l_Buffer+l_CurrBuffIndex, &l_Req);
            DEBUGPRINT("Add id %d to request.\n", l_Req.id);
        }
        NetworkCommon::sendWrapper(m_serverList[(*it)], l_Buffer, l_MessageSize);
 
        delete []l_Buffer;
    } 
    return 0;
}

int RobotClient::initGrid(const char* host, const char* port, const int id)
{
    int l_GridFd = initConnection(host, port);

    if (l_GridFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
		return -1;
    }
    m_Grids.push_back(l_GridFd);
    m_GridIdToFd[id] = l_GridFd;
    m_GridFdToId[l_GridFd] = id;

    return l_GridFd;
}

int RobotClient::initClock(const char* host, const char* port)
{
    int l_ClockFd = initConnection(host, port);

    if (l_ClockFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
		return -1;
    }

    m_ClockFd = l_ClockFd;

    return l_ClockFd;
}

int RobotClient::handleNewGrid(int id)
{

    DEBUGPRINT("Initializing a new grid.\n");
    //Header message;
    Msg_header l_Header = {SENDER_CLIENT, MSG_REQUESTINITTEAM};
    unsigned char *l_Buffer = new unsigned char[l_Header.size];

    if (l_Buffer == NULL)
    {
        ERRORPRINT("Couldn't allocate a buffer to store header\n");
        return -1;
    }
    NetworkCommon::packHeaderMessage(l_Buffer, &l_Header);
    
    int l_GridFd = m_GridIdToFd[id];
    TcpConnection* l_GridCon = m_serverList[l_GridFd];
    if (l_GridCon == NULL)
    {
        DEBUGPRINT("Error getting connection for grid id %d, mapped to fd %d\n", id, l_GridFd);
        return -1;
    }

    NetworkCommon::sendWrapper(m_serverList[l_GridFd], l_Buffer, l_Header.size);

	m_totalGridRequests++;
    delete l_Buffer;
    return 0;
}

int RobotClient::handler(int fd)
{
	DEBUGPRINT("Handling file descriptor: %i\n", fd);

    //Create a 'header' message and buffer to receive into.
    Msg_header l_Header;
    unsigned char *l_HeaderBuffer = new unsigned char[l_Header.size];

    if (l_HeaderBuffer == NULL)
    {
        ERRORPRINT("Couldn't allocate space for a header\n");
        return -1;
    }

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_serverList[fd];
        
    if (l_Conn == NULL)
    {
        DEBUGPRINT("Invalid connection for fd %d\n", fd);
        return -1;
    }
    //Receive the header message.
    NetworkCommon::recvWrapper(l_Conn, l_HeaderBuffer, l_Header.size);

    //Unpack the buffer into the 'header' message.
    unpack(l_HeaderBuffer, Msg_header_format, &l_Header.sender, &l_Header.message); 
    DEBUGPRINT("Received message %d from %d\n\n\n\n", l_Header.message, l_Header.sender);
    switch(l_Header.sender)
    {
        //Message is from clock.
        case(SENDER_CLOCK):
            switch(l_Header.message)
            {
                //Message is heart beat.
                case(MSG_HEARTBEAT) :
                {
                    time_t curr_sec = time(NULL); 
                    if (curr_sec > init_sec)
                    {
                        ERRORPRINT("Number of timesteps: %d\n", Timesteps);
                        init_sec = time(NULL);
                        Timesteps = 0;
                   }
                   Timesteps++;

                   DEBUGPRINT("Expecting to receive a heartbeat message from the clock.\n");
                   
                   //Create a heartbeat message and buffer to receive into.
                   Msg_HB l_HB;
                   unsigned char *l_hbBuffer = new unsigned char[l_HB.size]; 

                   if (l_hbBuffer == NULL)
                   {
                       ERRORPRINT("Couldn't allocate buffer space for receiving heartbeat\n");
                       return -1;
                   }
                  
                   //Receive the heartbeat.
                   NetworkCommon::recvWrapper(l_Conn, l_hbBuffer, l_HB.size); 

                   //Unpack heartbeat message from our buffer.
                   unpack(l_hbBuffer, Msg_HB_format, &l_HB.hb);
                   DEBUGPRINT("Hearbeat character: %hd\n", l_HB.hb);
                    
                   m_HeartBeat = l_HB.hb; 
                   if (sendRobotRequests() != 0)
                   {
                       DEBUGPRINT("Error sending robot sensor requests\n");
                   }

                   delete []l_hbBuffer;
               }
            }
            break;
        case(SENDER_GRIDSERVER):
            switch(l_Header.message)
            {
                case(MSG_RESPONDINITTEAM) :
                {
                    DEBUGPRINT("Receiving robot ids and positions for a team.\n");

                    Msg_TeamInit l_Team;
                    unsigned char* l_TeamBuffer = new unsigned char[l_Team.size];

                    if (l_TeamBuffer == NULL)
                    {
                        ERRORPRINT("Couldn't allocate space for receing team info\n");
                        return -1;
                    }

                    NetworkCommon::recvWrapper(l_Conn, l_TeamBuffer, l_Team.size);

                    unpack(l_TeamBuffer, Msg_TeamInit_format, &l_Team.id, &l_Team.x, &l_Team.y);

                    robotGameInstance->initTeam(l_Team); 
                    //Receive the number of robots we are expecting.
                    Msg_MsgSize l_NumRobots;
                    unsigned char l_NumRoboBuffer[l_NumRobots.size];

                    NetworkCommon::recvWrapper(l_Conn, l_NumRoboBuffer, l_NumRobots.size);

                    unpack(l_NumRoboBuffer, Msg_MsgSize_format, &l_NumRobots.msgSize);

                    DEBUGPRINT("Expecting ids and positions for %d robots.\n", l_NumRobots.msgSize);

                    int l_GridId = m_GridFdToId[fd];
                    //Receive the robot data.
                    Msg_InitRobot l_Robo;
                    unsigned int l_MessageSize = l_Robo.size*l_NumRobots.msgSize;
			
					unsigned int l_Offset = 0;
                    unsigned char* l_RoboBuffer = new unsigned char[l_MessageSize];

                    if (l_RoboBuffer == NULL)
                    {
                        ERRORPRINT("Couldn't allocate buffer space for receiving team robots\n");
                        return -1;
                    }

                    NetworkCommon::recvWrapper(l_Conn, l_RoboBuffer, l_MessageSize);
                    
                    //For each robot, recv, unpack, and add to game.
                    for (int i =0; i < l_NumRobots.msgSize; i++)
                    {
                        unpack(l_RoboBuffer+l_Offset, Msg_InitRobot_format, &l_Robo.id, &l_Robo.x, &l_Robo.y);
                        robotGameInstance->setTeamRobot(m_GridFdToId[fd], l_Team.id, l_Robo);
                        //insertRobot(l_GridId, l_Robo.id, l_Robo.x, l_Robo.y);
                        DEBUGPRINT("Received a new robot with ID %d, Coord (%f, %f)\n", 
                                l_Robo.id, l_Robo.x, l_Robo.y);
						l_Offset += l_Robo.size;
                    }
                    
                    l_NumRobots.msgSize;
                    m_totalGridResponses += 1;
					
					DEBUGPRINT("%lu %lu %lu\n", (unsigned long)m_totalGridRequests, (unsigned long)m_totalRobotsReceived, (unsigned long)m_totalGridResponses);
					
		        	/*if (m_totalGridRequests == m_totalGridResponses)
					{*/
						//DEBUGPRINT("ROBOT_CLIENT STATUS:\t Got all Grid repsonses for INIT TEAMS\n");
						

		    		unsigned char l_gridMessage[l_Header.size];
						
					if (NetworkCommon::packHeader(l_gridMessage, SENDER_CLIENT, MSG_CONFIRMTEAM))
					{
						DEBUGPRINT("ROBOT_CLIENT STATUS:\t Failed to pack header\n");
                        return -1;
					}
						
					if (NetworkCommon::sendMsg(l_gridMessage, l_Header.size , l_Conn) < 0)
					{
						DEBUGPRINT("ROBOT_CLIENT: STATUS\t Could not send the robot size message\n");
					}
						
					delete []l_RoboBuffer;	
				    delete []l_TeamBuffer;	
                    return 0;
                }
                break;
                case(MSG_RESPONDPROCESSACTION) :
                {
                    int gridId = m_GridFdToId[fd];
                    TcpConnection * l_curConnection = m_serverList[fd];
                    
                    DEBUGPRINT("Received action responses from grid: %d\n", gridId);
                    
                    Msg_MsgSize_32 l_moveObjectCountMsg = {0};
                    Msg_MsgSize_32 l_dropObjectCountMsg = {0};
                    Msg_MsgSize_32 l_pickupObjectCountMsg = {0};
                    
                    Msg_Response_Movement l_moveMsg;
                    Msg_Response_Drop l_dropMsg;
                    Msg_Response_Pickup l_pickupMsg;
                    
                    if (NetworkCommon::recvMessageSize32(l_moveObjectCountMsg, l_curConnection) < 0)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to receive message size\n");
                    	return -1;
                    }
                    

                    if (NetworkCommon::recvMessageSize32(l_dropObjectCountMsg, l_curConnection) < 0)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to receive message size\n");
                    	return -1;
                    }
                    
 
                    if (NetworkCommon::recvMessageSize32(l_pickupObjectCountMsg, l_curConnection) < 0)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to receive message size\n");
                    	return -1;
                    }            
           
                    DEBUGPRINT("Received %d MOVEMENT actions for grid %d \n", l_moveObjectCountMsg.msgSize, gridId);
                    DEBUGPRINT("Received %d DROP actions for grid %d \n", l_dropObjectCountMsg.msgSize,gridId);
                    DEBUGPRINT("Received %d PICKUP actions for grid %d \n", l_pickupObjectCountMsg.msgSize, gridId);                    
                    
 					unsigned int l_MessageSize = l_moveObjectCountMsg.msgSize*l_moveMsg.size;
                    l_MessageSize += l_dropObjectCountMsg.msgSize*l_dropMsg.size;
                    l_MessageSize += l_pickupObjectCountMsg.msgSize*l_pickupMsg.size;
                    
                    DEBUGPRINT("GRID_SERVER ERROR:\t messagesize: %i\n", l_MessageSize);
                    
                    unsigned char * l_ResultsBuffer = new unsigned char[l_MessageSize];
                    
                    if (l_ResultsBuffer == NULL)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to allocate memory for process action\n");
                    	return -1;
                    }
                    
                    DEBUGPRINT("recv\n");
                    
                  	if (l_curConnection->recv(l_ResultsBuffer, l_MessageSize) < 0)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Could not get Msg_Actions for processs actions\n");
						return -1;
					}

					DEBUGPRINT("GRID_SERVER ERROR:\t done recv\n");			
			
					std::vector<Msg_Response_Movement> l_moveResponse;
					std::vector<Msg_Response_Drop> l_dropResponse;
					std::vector<Msg_Response_Pickup> l_pickupResponse;
                    
                    unsigned int l_Offset = 0;
                   
                    // pack movement actions
                    for (int i = 0; i <  l_moveObjectCountMsg.msgSize; i++)
                    {
                    	Msg_Response_Movement l_responseMovement;
                    	
                       unpack(l_ResultsBuffer+l_Offset, Msg_Response_Movement_format, &l_responseMovement.robotId, 
                        		&l_responseMovement.xPos, &l_responseMovement.yPos, &l_responseMovement.orientation,
                        		&l_responseMovement.gridId);
                        
                        l_moveResponse.push_back(l_responseMovement);
                        
                        DEBUGPRINT("ROBOT_CLIENT STATUS:\t Packing MOVE Robot ID:%u, xPos:%f yPos:%f orientation:%f gridId:%d\n", l_responseMovement.robotId, 
                        		l_responseMovement.xPos, l_responseMovement.yPos, l_responseMovement.orientation,
                        		l_responseMovement.gridId);
                        		
                        l_Offset += l_responseMovement.size;
                    }
					
                    // pack movement actions
                    for (int i = 0; i <  l_dropObjectCountMsg.msgSize; i++)
                    {
                    	Msg_Response_Drop l_responseDrop;
                    	
                       	unpack(l_ResultsBuffer+l_Offset, Msg_Response_Drop_format, &l_responseDrop.robotId);
                        
                        l_dropResponse.push_back(l_responseDrop);
                        
                        DEBUGPRINT("ROBOT_CLIENT STATUS:\t Unpacking DROP Robot ID:%u\n", l_responseDrop.robotId);
                        		
                        l_Offset += l_responseDrop.size;
                    }
                    
                    // pack movement actions
                    for (int i = 0; i <  l_pickupObjectCountMsg.msgSize; i++)
                    {
                    	Msg_Response_Pickup l_responsePickup;
                    	
                       	unpack(l_ResultsBuffer+l_Offset, Msg_Response_Pickup_format, &l_responsePickup.robotId);
                        
                        l_pickupResponse.push_back(l_responsePickup);
                        
                        DEBUGPRINT("ROBOT_CLIENT STATUS:\t Packing PICKUP Robot ID:%u\n", l_responsePickup.robotId);
                        		
                        l_Offset += l_responsePickup.size;
                    }                    

                    robotGameInstance->actionResult(&l_moveResponse, &l_dropResponse, &l_pickupResponse);

                    m_ReadyActionGrids++;
                    if (m_ReadyActionGrids == m_Grids.size())
                    {
                        TcpConnection* l_ClockConn = m_serverList[m_ClockFd];

                        if (l_ClockConn == NULL)
                        {
                            ERRORPRINT("Error in getting the clock connection\n");
                            return -1;
                        }
                        //Prepare our 'header' message.
                        Msg_header l_Header = {SENDER_CLIENT, MSG_HEARTBEAT};
                        Msg_HB l_HB = {m_HeartBeat};

                        unsigned int l_MessageSize = l_Header.size+l_HB.size;
                        unsigned char* l_HBBuffer = new unsigned char[l_MessageSize];

                        if (l_HBBuffer == NULL)
                        {
                            ERRORPRINT("Error creating buffer for sending heartbeat\n");
                            return -1;
                        }
                        NetworkCommon::packHeaderMessage(l_HBBuffer, &l_Header);
                      
                        //Pack the hearbeat into the header message buffer.
                        if (pack(l_HBBuffer+l_Header.size, Msg_HB_format, l_HB.hb) != l_HB.size)
                        {
                            DEBUGPRINT("Failed to pack the HB message\n");
                            return -1;
                        }
                        NetworkCommon::sendWrapper(l_ClockConn, l_HBBuffer, l_MessageSize);
                        DEBUGPRINT("Sent heartbeat.\n");
                        m_ReadyActionGrids = 0;

                        delete[]l_HBBuffer;
                    }

                    delete []l_ResultsBuffer;

                    break;
                }
                case(MSG_RESPONDSENSORDATA) :
                {
                    std::vector <std::pair <uid, std::vector<Msg_SensedObjectGroupItem > > > l_SensedInfo;

                    //Receive the total number of robots we are getting sens info for.
                    Msg_MsgSize l_NumRobots;
                    unsigned char l_NumRobBuffer[l_NumRobots.size];

                    NetworkCommon::recvWrapper(l_Conn, l_NumRobBuffer, l_NumRobots.size);
                    unpack(l_NumRobBuffer, Msg_MsgSize_format, &l_NumRobots.msgSize);

                    DEBUGPRINT("Expecting %d robots.\n", l_NumRobots.msgSize );

                    //Go through all of the robots we expect info for.
                    for (int i = 0; i < l_NumRobots.msgSize;i++)
                    {
                        //First get the header telling us how many objects are sensed and for which object.
                        Msg_SensedObjectGroupHeader l_RoboHeader;
                        unsigned char *l_GroupHeaderBuff = new unsigned char[l_RoboHeader.size];

                        if (l_GroupHeaderBuff == NULL)
                        {
                            ERRORPRINT("Error allocating space for the group header in respond sensor data\n");
                            return -1;
                        }
                        NetworkCommon::recvWrapper(l_Conn, l_GroupHeaderBuff, l_RoboHeader.size);
                        unpack(l_GroupHeaderBuff, Msg_SensedObjectGroupHeader_format,
                                &l_RoboHeader.id, &l_RoboHeader.objectCount);

                        DEBUGPRINT("Robot %d, with ID %d, has sensed %d objects\n",
                                i, l_RoboHeader.id, l_RoboHeader.objectCount);

                        
                        std::vector<Msg_SensedObjectGroupItem> l_SensedRobos;
                        //Receive all of our sensed items.
                        for (int a = 0; a < l_RoboHeader.objectCount; a++)
                        {
                            
                           //Receive a sensed item.
                           Msg_SensedObjectGroupItem l_SensedItem;
                           unsigned char * l_SensedItemBuffer = new unsigned char[l_SensedItem.size]; 

                           if (l_SensedItemBuffer == NULL)
                           {
                               ERRORPRINT("Couldn't allocate memory for sensed item buffer\n");
                               return -1;
                           }
                            
                           //Receive and insert into our map of sensed items.
                           NetworkCommon::recvWrapper(l_Conn, l_SensedItemBuffer, l_SensedItem.size);
                           unpack(l_SensedItemBuffer, Msg_SensedObjectGroupItem_format, &l_SensedItem.id,
                                   &l_SensedItem.x, &l_SensedItem.y);
                           l_SensedRobos.push_back(l_SensedItem);

                           DEBUGPRINT("Sensed object with ID %d, pos(%d, %d)\n", l_SensedItem.id,
                                   l_SensedItem.x, l_SensedItem.y);
                           delete []l_SensedItemBuffer;
                        }
                        
                        std::pair<uid, std::vector<Msg_SensedObjectGroupItem > > l_RoboInfo (l_RoboHeader.id, l_SensedRobos);
                        l_SensedInfo.push_back(l_RoboInfo);
                        delete []l_GroupHeaderBuff;
                    }
                    robotGameInstance->receiveSensorData(&l_SensedInfo);
					// don't need to optimize the above code for this case it already is
                    m_ReadyGrids++; 
                    DEBUGPRINT("Received sensory data from a grid. %d Ready, %zu total\n", m_ReadyGrids, m_Grids.size());
                    if (m_ReadyGrids == m_Grids.size())
                    {   
                        DEBUGPRINT("Finished receiving sensory info.\n");
                        std::vector<Msg_Request_Movement> l_move;
                        std::vector<Msg_Request_Pickup> l_pickup;
                        std::vector<Msg_Request_Drop> l_drop;
                        
                        std::vector<int>::const_iterator end = m_Grids.end();
                        DEBUGPRINT("Getting actions from simulation\n");
                        for(std::vector<int>::const_iterator it = m_Grids.begin(); it != end; it++)
                        {
                            int gridId = m_GridFdToId[(*it)];
                            int gridFd = (*it);
                            DEBUGPRINT("Grid ID: %d, Grid FD: %d\n", gridId, gridFd);
                            
                            robotGameInstance->sendAction(gridId, &l_move, &l_drop,  &l_pickup);
                            
                            uint32_t l_moveSize = l_move.size();
                            uint32_t l_dropSize = l_drop.size();
                            uint32_t l_pickupSize = l_pickup.size();

                            DEBUGPRINT("Received %zu MOVEMENT actions for grid %d \n", l_moveSize, gridId);
                            DEBUGPRINT("Received %zu DROP actions for grid %d \n", l_dropSize, gridId);
                            DEBUGPRINT("Received %zu PICKUP actions for grid %d \n", l_pickupSize, gridId);
                            
                            // Prepair to send the information
                            
                            Msg_header l_Header = {SENDER_CLIENT, MSG_PROCESSACTION};
                            
                            Msg_MsgSize_32 l_moveObjectCountMsg = {l_moveSize};
                            Msg_MsgSize_32 l_dropObjectCountMsg = {l_dropSize};
                            Msg_MsgSize_32 l_pickupObjectCountMsg = {l_pickupSize};
                            
                            Msg_Request_Movement l_moveMsg;
                            Msg_Request_Drop l_dropMsg;
                            Msg_Request_Pickup l_pickupMsg;
                            

                            unsigned int l_MessageSize = l_Header.size;
                            l_MessageSize += l_moveObjectCountMsg.size + l_dropObjectCountMsg.size + l_pickupObjectCountMsg.size;
                            l_MessageSize += (l_moveSize * l_moveMsg.size);
                            l_MessageSize += (l_dropSize * l_dropMsg.size);
                            l_MessageSize += (l_pickupSize * l_pickupMsg.size);
                            
                            unsigned char *l_ActionBuffer = new unsigned char[l_MessageSize];

                            if (l_ActionBuffer == NULL)
                            {
                                ERRORPRINT("Error allocating buffer for sending actions \n");
                                return -1;
                            }
                            
                            unsigned int l_Offset = 0;

                            NetworkCommon::packHeaderMessage(l_ActionBuffer+l_Offset, &l_Header);
                            l_Offset += l_Header.size;

                            if (pack(l_ActionBuffer+l_Offset, Msg_MsgSize_32_format, l_moveObjectCountMsg.msgSize) != l_moveObjectCountMsg.size)
                            {
                               ERRORPRINT("ROBOT_CLIENT ERROR:\t Error packing MOVEMENT size\n");
                               return -1;
                            }
                            
                            l_Offset += l_moveObjectCountMsg.size;

						
							if (pack(l_ActionBuffer+l_Offset, Msg_MsgSize_32_format, l_dropObjectCountMsg.msgSize) != l_dropObjectCountMsg.size)
                            {
                               ERRORPRINT("ROBOT_CLIENT ERROR:\t Error packing MOVEMENT size\n");
                               return -1;
                            }                            
                            l_Offset += l_dropObjectCountMsg.size;
                            
							if (pack(l_ActionBuffer+l_Offset, Msg_MsgSize_32_format, l_pickupObjectCountMsg.msgSize) != l_pickupObjectCountMsg.size)
                            {
                               ERRORPRINT("ROBOT_CLIENT ERROR:\t Error packing MOVEMENT size\n");
                               return -1;
                            }  
                                                      
                            l_Offset += l_pickupObjectCountMsg.size;
                            
                            DEBUGPRINT("ROBOT_CLIENT STATUS:\t Message size:%d after header + size MOVE DROP PICKUP\n", l_Offset);
                            

                            for (int i = 0; i <  l_move.size(); i++)
                            {
                                if (pack(l_ActionBuffer+l_Offset, Msg_Request_Movement_format, l_move.at(i).robotId, 
                                		l_move.at(i).forwardSpeed, l_move.at(i).rotationSpeed)
                                			!= l_moveMsg.size
                                	)
                                {
                                    ERRORPRINT("ROBOT_CLIENT ERROR:\t Error packing robo actions\n");
                                    return -1;
                                }
                                
                                DEBUGPRINT("ROBOT_CLIENT STATUS:\t Packing MOVE Robot ID:%u, forwardSpeed:%f rotationalSpeed:%f\n", l_move.at(i).robotId, 
                                	l_move.at(i).forwardSpeed, l_move.at(i).rotationSpeed);
                                l_Offset += l_moveMsg.size;
                            }
                         
                         	DEBUGPRINT("ROBOT_CLIENT STATUS:\t Message size:%d after MOVE\n", l_Offset);
                         	
							for (int i = 0; i < l_dropSize; i++)
                            {
                                if (pack(l_ActionBuffer+l_Offset, Msg_Request_Drop_format, l_drop.at(i).robotId)
                                			!= l_dropMsg.size
                                	)
                                {
                                    ERRORPRINT("ROBOT_CLIENT ERROR:\t Error packing robo actions\n");
                                    return -1;
                                }
                                 DEBUGPRINT("ROBOT_CLIENT STATUS:\t Packing DROP Robot ID:%u\n", l_drop.at(i).robotId);
                                l_Offset += l_dropMsg.size;
                            }  
                            
                            DEBUGPRINT("ROBOT_CLIENT STATUS:\t Message size:%d after DROP\n", l_Offset);
                            
							for (int i = 0; i < l_pickupSize; i++)
                            {
                                if (pack(l_ActionBuffer+l_Offset, Msg_Request_Pickup_format, l_pickup.at(i).robotId,
                                			l_pickup.at(i).puckId) != l_pickupMsg.size
                                	)
                                {
                                    ERRORPRINT("ROBOT_CLIENT ERROR:\t Error packing robo actions\n");
                                    return -1;
                                }
                                 DEBUGPRINT("ROBOT_CLIENT STATUS:\t Packing PICKUP Robot ID:%u, puckId:%u\n", l_pickup.at(i).robotId, 
                                	l_pickup.at(i).puckId);
                                l_Offset += l_dropMsg.size;
                            }                                                              
                            
                            DEBUGPRINT("ROBOT_CLIENT STATUS:\t Message size:%d after PICKUP\n", l_Offset);
                            
                           
                            NetworkCommon::sendWrapper(m_serverList[(*it)],l_ActionBuffer, l_MessageSize);
                            
                            DEBUGPRINT("ROBOT_CLIENT STATUS:\t Sent process action request to grid server\n");
                            

                            l_move.clear();
							l_drop.clear();
							l_pickup.clear();

							
							
                            delete[] l_ActionBuffer;

                        }
                        m_ReadyGrids = 0;
                    }
                     
                }
            }
            break;
    }


    delete[] l_HeaderBuffer;

    return 0;
}

void RobotClient::setFOV(float fov)
{
	m_robot_FOV = fov;
}

void RobotClient::setRadius(float radius)
{
	m_robot_Radius = radius;
}

void RobotClient::setSensorRange(float sensorRange)
{
	m_robot_SensorRange = sensorRange;
}
void RobotClient::setPickupRange(float pickupRange)
{
	m_robot_PickupRange = pickupRange;
}

void RobotClient::setWorldSize(float worldSize)
{
	m_robot_WorldSize = worldSize;
}

void RobotClient::setHomeRadius(float homeRadius)
{
	m_robot_HomeRadius = homeRadius;
}
