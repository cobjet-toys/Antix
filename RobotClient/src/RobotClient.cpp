#include "RobotClient.h"
#include "Messages.h"
#include <string.h>
#include "Config.h"
#include <map>
#include <vector>
#include "Types.h"

#ifdef DEBUG
#include <time.h>
#include <networkCommon.h>
#endif

using namespace Network;


static time_t init_sec = time(NULL);
static int Timesteps = 0;


RobotClient::RobotClient():Client(), m_ReadyGrids(0)
{
    robotGameInstance = new RobotGame();

    // make a robot_info vector for testing
    std::vector<uid>* robot_ids;
    robotGameInstance->requestSensorData(1, robot_ids);
    
	m_totalGridRequests = 0;
	m_totalGridResponses = 0;
	m_totalRobotsReceived = 0;
}

int RobotClient::sendWrapper(TcpConnection * conn, unsigned char* buffer, int msgSize)
{
    if (conn->send(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to send a message.\n");
        return -1;
    }
    return 0;
}

int RobotClient::recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize)
{
    if (conn->recv(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to receive a message.\n");
        return -1;
    }
    return 0;
}

int RobotClient::packHeaderMessage(unsigned char* buffer, uint16_t sender, uint16_t message)
{
    Msg_header l_Header;

    l_Header.sender = sender;
    l_Header.message = message;

    if (pack(buffer, Msg_header_format, l_Header.sender, l_Header.message) != l_Header.size)
    {
        DEBUGPRINT("Failed to pack a header message. Sender: %d, Message: %d", sender, message);
        return -1;
    }
    return 0;
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
        robotGameInstance->requestSensorData((*it), &l_RobotIds);
        
        l_Size.msgSize = 4000;//REPLACE WITH ACTUAL REQUEST FOR ROBOTS(l_RobotIds.size())

        unsigned int l_MessageSize = (l_Size.msgSize*l_Req.size)+l_Header.size+l_Size.size;
        unsigned char l_Buffer[l_MessageSize];

        //Keep track of where in the buffer we are packing into.
        unsigned int l_CurrBuffIndex = 0; 

        //Add our header.
        packHeaderMessage(l_Buffer+l_CurrBuffIndex, SENDER_CLIENT, MSG_REQUESTSENSORDATA);        
        l_CurrBuffIndex += l_Header.size;

         //Add the number of elements we are sending
        pack(l_Buffer+l_CurrBuffIndex, Msg_MsgSize_format, l_Size.msgSize);
        l_CurrBuffIndex += l_Size.size;
               
        for (int i = 0; i < l_Size.msgSize;i++)
        {
            l_Req.id = 1234; //REPLACE WITH ACTUAL ROBOT ID l_Req.id = l_RobotIds[i];
            if (pack(l_Buffer+l_CurrBuffIndex, Msg_RequestSensorData_format, l_Req.id) != l_Req.size)
            {
                DEBUGPRINT("Error packing sensor request into the buffer\n");
                return -1;
            }
            l_CurrBuffIndex += l_Req.size;
        }
        sendWrapper(m_serverList[(*it)], l_Buffer, l_MessageSize);
 
        unpack(l_Buffer, Msg_header_format, &l_Header.sender, &l_Header.message);
        DEBUGPRINT("Sender: %d Message: %d\n", l_Header.sender, l_Header.message);
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

    printf("Initializing a new grid.\n");
    //Header message;
    Msg_header l_Header;
    unsigned char l_Buffer[l_Header.size];
    packHeaderMessage(l_Buffer, SENDER_CLIENT, MSG_REQUESTINITTEAM);
    
    int l_GridFd = m_GridIdToFd[id];
    sendWrapper(m_serverList[l_GridFd], l_Buffer, l_Header.size);
	m_totalGridRequests++;
}

int RobotClient::handler(int fd)
{
	DEBUGPRINT("Handling file descriptor: %i\n", fd);

    //Create a 'header' message and buffer to receive into.
    Msg_header l_Header;
    unsigned char l_HeaderBuffer[l_Header.size];

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_serverList[fd];
        
    //Receive the header message.
    recvWrapper(l_Conn, l_HeaderBuffer, l_Header.size);

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
                        printf("Number of timesteps: %d\n", Timesteps);
                        init_sec = time(NULL);
                        Timesteps = 0;
                   }
                   Timesteps++;

                   DEBUGPRINT("Expecting to receive a heartbeat message from the clock.\n");
                   
                   //Create a heartbeat message and buffer to receive into.
                   Msg_HB l_HB;
                   unsigned char l_hbBuffer[l_HB.size]; 
                  
                   //Receive the heartbeat.
                   recvWrapper(l_Conn, l_hbBuffer, l_HB.size); 

                   //Unpack heartbeat message from our buffer.
                   unpack(l_hbBuffer, Msg_HB_format, &l_HB.hb);
                   DEBUGPRINT("Hearbeat character: %hd\n", l_HB.hb);
                    
                   m_HeartBeat = l_HB.hb; 
                   if (sendRobotRequests() != 0)
                   {
                       DEBUGPRINT("Error sending robot sensor requests\n");
                   }
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
                    unsigned char l_TeamBuffer[l_Team.size];

                    recvWrapper(l_Conn, l_TeamBuffer, l_Team.size);

                    unpack(l_TeamBuffer, Msg_TeamInit_format, &l_Team.id, &l_Team.x, &l_Team.y);

                    robotGameInstance->initTeam(l_Team); 
                    //Receive the number of robots we are expecting.
                    Msg_MsgSize l_NumRobots;
                    unsigned char l_NumRoboBuffer[l_NumRobots.size];

                    recvWrapper(l_Conn, l_NumRoboBuffer, l_NumRobots.size);

                    unpack(l_NumRoboBuffer, Msg_MsgSize_format, &l_NumRobots.msgSize);

                    DEBUGPRINT("Expecting ids and positions for %d robots.\n", l_NumRobots.msgSize);

                    int l_GridId = m_GridFdToId[fd];
                    //Receive the robot data.
                    Msg_InitRobot l_Robo;
                    unsigned int l_MessageSize = l_Robo.size*l_NumRobots.msgSize;
					printf("%ui\n", l_MessageSize);
			
					unsigned int l_Offset = 0;
                    unsigned char l_RoboBuffer[l_MessageSize];

                    recvWrapper(l_Conn, l_RoboBuffer, l_MessageSize);
                    
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
					
					printf("%lu %lu %lu\n", (unsigned long)m_totalGridRequests, (unsigned long)m_totalRobotsReceived, (unsigned long)m_totalGridResponses);
					
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
						
						
					
					DEBUGPRINT("derp?\n");
					//return 0;
                }
                break;
                case(MSG_RESPONDSENSORDATA) :
                {
                    
                    std::map<uid, std::vector<Msg_SensedObjectGroupItem> > l_SensedInfo;

                    //Receive the total number of robots we are getting sens info for.
                    Msg_MsgSize l_NumRobots;
                    unsigned char l_NumRobBuffer[l_NumRobots.size];

                    recvWrapper(l_Conn, l_NumRobBuffer, l_NumRobots.size);
                    unpack(l_NumRobBuffer, Msg_MsgSize_format, &l_NumRobots.msgSize);

                    DEBUGPRINT("Expecting %d robots.\n", l_NumRobots.msgSize );

                    //Go through all of the robots we expect info for.
                    for (int i = 0; i < l_NumRobots.msgSize;i++)
                    {
                        //First get the header telling us how many objects are sensed and for which object.
                        Msg_SensedObjectGroupHeader l_RoboHeader;
                        unsigned char l_GroupHeaderBuff[l_RoboHeader.size];

                        recvWrapper(l_Conn, l_GroupHeaderBuff, l_RoboHeader.size);
                        unpack(l_GroupHeaderBuff, Msg_SensedObjectGroupHeader_format,
                                &l_RoboHeader.id, &l_RoboHeader.objectCount);

                        DEBUGPRINT("Robot %d, with ID %d, has sensed %d objects\n",
                                i, l_RoboHeader.id, l_RoboHeader.objectCount);
                        
                        std::vector<Msg_SensedObjectGroupItem> &l_Info = l_SensedInfo[l_RoboHeader.id];
                        //Receive all of our sensed items.
                        for (int a = 0; a < l_RoboHeader.objectCount; a++)
                        {
                            
                           //Receive a sensed item.
                           Msg_SensedObjectGroupItem l_SensedItem;
                           unsigned char l_SensedItemBuffer[l_SensedItem.size]; 
                            
                           //Receive and insert into our map of sensed items.
                           recvWrapper(l_Conn, l_SensedItemBuffer, l_SensedItem.size);
                           unpack(l_SensedItemBuffer, Msg_SensedObjectGroupItem_format, &l_SensedItem.id,
                                   &l_SensedItem.x, &l_SensedItem.y);
                           l_Info.push_back(l_SensedItem);

                           DEBUGPRINT("Sensed object with ID %d, pos(%d, %d)\n", l_SensedItem.id,
                                   l_SensedItem.x, l_SensedItem.y);
                        }
                    }
                    //TODO
                    //robotGameInstance->receiveSensorData(&l_SensedInfo);

                    m_ReadyGrids++; 
                    DEBUGPRINT("Recevied sensory data from a grid. %d Ready, %zu total\n", m_ReadyGrids, m_Grids.size());
                    if (m_ReadyGrids == m_Grids.size())
                    {   
                        DEBUGPRINT("Finished one loop\n");
                        TcpConnection* l_ClockConn = m_serverList[m_ClockFd];
                        //Prepare our 'header' message.
                        Msg_header l_Header;
                        Msg_HB l_HB = {m_HeartBeat};

                        unsigned int l_MessageSize = l_Header.size+l_HB.size;
                        unsigned char l_HBBuffer[l_MessageSize];
                        packHeaderMessage(l_HBBuffer, SENDER_CLIENT, MSG_HEARTBEAT);
                        
                        //Pack the hearbeat into the header message buffer.
                        if (pack(l_HBBuffer+l_Header.size, Msg_HB_format, l_HB.hb) != l_HB.size)
                        {
                            DEBUGPRINT("Failed to pack the HB message\n");
                            return -1;
                        }
                        sendWrapper(l_ClockConn, l_HBBuffer, l_MessageSize);
                        DEBUGPRINT("Sent heartbeat.\n");

                        m_ReadyGrids = 0;
                    }
                     
                }
            }
            break;
    }


    return 0;
}
