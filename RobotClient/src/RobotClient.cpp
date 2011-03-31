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
    robotGameInstance = new RobotGame();

    // make a robot_info vector for testing
    std::vector<uid>* robot_ids;
    robotGameInstance->requestSensorData(1, robot_ids);
    
	m_totalGridRequests = 0;
	m_totalGridResponses = 0;
	m_totalRobotsReceived = 0;
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
        DEBUGPRINT("Requesting sensory info from grid id: %d\n", m_GridFdToId[(*it)]);
        robotGameInstance->requestSensorData(m_GridFdToId[(*it)], &l_RobotIds);
        
        l_Size.msgSize = l_RobotIds.size();//REPLACE WITH ACTUAL REQUEST FOR ROBOTS(l_RobotIds.size())
        DEBUGPRINT("Requesting sensory info for %zu robots\n", l_RobotIds.size());

        unsigned int l_MessageSize = (l_Size.msgSize*l_Req.size)+l_Header.size+l_Size.size;
        unsigned char* l_Buffer = new unsigned char[l_MessageSize];

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
 
        delete l_Buffer;
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
    Msg_header l_Header = {SENDER_CLIENT, MSG_REQUESTINITTEAM};
    unsigned char l_Buffer[l_Header.size];
    NetworkCommon::packHeaderMessage(l_Buffer, &l_Header);
    
    int l_GridFd = m_GridIdToFd[id];
    NetworkCommon::sendWrapper(m_serverList[l_GridFd], l_Buffer, l_Header.size);
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
                   NetworkCommon::recvWrapper(l_Conn, l_hbBuffer, l_HB.size); 

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
					printf("%ui\n", l_MessageSize);
			
					unsigned int l_Offset = 0;
                    unsigned char l_RoboBuffer[l_MessageSize];

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
                case(MSG_RESPONDPROCESSACTION) :
                {
                    int gridId = m_GridFdToId[fd];
                    DEBUGPRINT("Received action responses from grid: %d\n", gridId);
                    
                    Msg_MsgSize l_Size;
                    Msg_RobotInfo l_Result;

                    unsigned char l_SizeBuffer[l_Size.size];

                    NetworkCommon::recvWrapper(l_Conn, l_SizeBuffer, l_Size.size);

                    unpack(l_SizeBuffer, Msg_MsgSize_format, &l_Size.msgSize);

                    DEBUGPRINT("Expecting %d results robos\n", l_Size.msgSize);

                    unsigned int l_MessageSize = l_Size.msgSize*l_Result.size;
                    unsigned char l_RobotInfoBuff[l_MessageSize];

                    NetworkCommon::recvWrapper(l_Conn, l_RobotInfoBuff, l_MessageSize);

                    unsigned int l_Offset = 0;
                    std::vector<Msg_RobotInfo> l_Results;
                    for (int i =0; i < l_Size.msgSize; i++)
                    {
                        unpack(l_RobotInfoBuff+l_Offset, Msg_RobotInfo_format, &l_Result.robotid, &l_Result.x_pos, &l_Result.y_pos, &l_Result.speed, &l_Result.angle, &l_Result.puckid, &l_Result.gridid);
                        l_Results.push_back(l_Result);
                        l_Offset += l_Result.size;
                        DEBUGPRINT("NEW ROBOTINFO: %d, %f, %f \n", l_Result.robotid, l_Result.x_pos, l_Result.y_pos);
                    }
                    
                    robotGameInstance->actionResult(&l_Results);

                    m_ReadyActionGrids++;
                    if (m_ReadyActionGrids == m_Grids.size())
                    {
                        TcpConnection* l_ClockConn = m_serverList[m_ClockFd];
                        //Prepare our 'header' message.
                        Msg_header l_Header = {SENDER_CLIENT, MSG_HEARTBEAT};
                        Msg_HB l_HB = {m_HeartBeat};

                        unsigned int l_MessageSize = l_Header.size+l_HB.size;
                        unsigned char l_HBBuffer[l_MessageSize];
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
                    }
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
                        unsigned char l_GroupHeaderBuff[l_RoboHeader.size];

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
                           unsigned char l_SensedItemBuffer[l_SensedItem.size]; 
                            
                           //Receive and insert into our map of sensed items.
                           NetworkCommon::recvWrapper(l_Conn, l_SensedItemBuffer, l_SensedItem.size);
                           unpack(l_SensedItemBuffer, Msg_SensedObjectGroupItem_format, &l_SensedItem.id,
                                   &l_SensedItem.x, &l_SensedItem.y);
                           l_SensedRobos.push_back(l_SensedItem);

                           DEBUGPRINT("Sensed object with ID %d, pos(%d, %d)\n", l_SensedItem.id,
                                   l_SensedItem.x, l_SensedItem.y);
                        }
                        
                        std::pair<uid, std::vector<Msg_SensedObjectGroupItem > > l_RoboInfo (l_RoboHeader.id, l_SensedRobos);
                        l_SensedInfo.push_back(l_RoboInfo);
                    }
                    robotGameInstance->receiveSensorData(&l_SensedInfo);

                    m_ReadyGrids++; 
                    DEBUGPRINT("Received sensory data from a grid. %d Ready, %zu total\n", m_ReadyGrids, m_Grids.size());
                    if (m_ReadyGrids == m_Grids.size())
                    {   
                        DEBUGPRINT("Finished receiving sensory info.\n");
                        std::vector<Msg_Action> l_RoboActions;
                        std::vector<int>::const_iterator end = m_Grids.end();
                        DEBUGPRINT("Getting actions from simulation\n");
                        for(std::vector<int>::const_iterator it = m_Grids.begin(); it != end; it++)
                        {
                            int gridId = m_GridFdToId[(*it)];
                            int gridFd = (*it);
                            DEBUGPRINT("Grid ID: %d, Grid FD: %d\n", gridId, gridFd);
                            
                            robotGameInstance->sendAction(gridId, &l_RoboActions);
                            DEBUGPRINT("Received %zu actions for grid %d \n", l_RoboActions.size(), gridId);
                            
                            Msg_header l_Header = {SENDER_CLIENT, MSG_PROCESSACTION};
                            Msg_MsgSize l_Size = {l_RoboActions.size()};
                            Msg_Action l_Action;

                            unsigned int l_MessageSize = l_Header.size+l_Size.size+ (l_Action.size*l_RoboActions.size());
                            unsigned char l_ActionBuffer[l_MessageSize];
                            unsigned int l_Offset = 0;

                            NetworkCommon::packHeaderMessage(l_ActionBuffer+l_Offset, &l_Header);
                            l_Offset += l_Header.size;

                            pack(l_ActionBuffer+l_Offset, Msg_MsgSize_format, l_Size.msgSize);
                            l_Offset += l_Size.size;

                            for (int i = 0; i < l_RoboActions.size(); i++)
                            {
                                pack(l_ActionBuffer+l_Offset, Msg_Action_format, l_RoboActions[i].robotid, l_RoboActions[i].action, l_RoboActions[i].speed,l_RoboActions[i].angle);
                                l_Offset += l_Action.size;
                            }
                            NetworkCommon::sendWrapper(m_serverList[(*it)],l_ActionBuffer, l_MessageSize);
                            DEBUGPRINT("Sent process action request to grid server\n");
                            l_RoboActions.clear();
                        }
                        m_ReadyGrids = 0;
                    }
                     
                }
            }
            break;
    }


    return 0;
}
