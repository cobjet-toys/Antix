#include "GridServer.h"
#include "Config.h"
#include "Types.h"
#include <map>
#include <vector>
#include "networkCommon.h"
#include <stdlib.h>
#include <pthread.h>

using namespace Network;

GridServer::GridServer():Server()
{
    m_uId = -1;
    m_hb_rcvd = 0;
	m_idRangeFrom = 0;
	m_idRangeTo = 0;
	m_robotsPerTeam = 0;
	m_teamsAvailable = 0;
	m_robotsAvailable = 0;
	m_robotsConfirmed = 0;
	m_ControllerFd = -1;
	m_teamsConfirmed =0;	
	m_drawerConn = 0;
	m_updateDrawerFlag = 0;
    m_Hb = 0;
    m_ReadyPartners = 0;
    m_ClockFd = 0;
    m_numClients = 0;
}

int GridServer::initGridGame()
{
    /*
	gridGameInstance = new GridGame(m_uId, m_teamsAvailable, m_robotsPerTeam, m_idRangeFrom, m_idRangeTo); // needs to not do this in grid game constructor!
    */

    #ifdef GRIDTEST

	printf("id=%lu, teams=%lu, robots=%lu, idfrom=%lu, idto=%lu\n", (unsigned long)m_uId, (unsigned long)m_teamsAvailable,(unsigned long) m_robotsPerTeam, (unsigned long)m_idRangeFrom, (unsigned long)m_idRangeTo);
 
    // parameters: gridid, num_of_teams, robots_per_team, id_from, id_to
	gridGameInstance = new GridGame(1, 2, 5, 10, 19, 0.2, 10, 2, 10); 
	GridGame* gridGameInstance2 = new GridGame(2, 2, 5, 10, 19, 0.2, 10, 2, 10); 
    //GridGame* gridGameInstance2 = new GridGame(2, 2, 5, 20, 29);

    Msg_RobotInfo newrobot;
    newrobot.robotid = 400;
    newrobot.x_pos = 2.5;
    newrobot.y_pos = 2.5;

    std::vector<unsigned int> robots;
    robots.push_back(10);
    robots.push_back(14);

    std::vector<RobotSensedObjectsPair>* sensed_items = new std::vector<RobotSensedObjectsPair>;

    //DEBUGPRINT("=====Create Game\n");

    //DEBUGPRINT("=====Initialize teams\n");
    //std::vector<robot_info>* robot_info_vector;
    //gridGameInstance->initializeTeam(teams, robot_info_vector);
    DEBUGPRINT("=====Printing initial population of grid1 after initializing gridGameInstance\n");
    //gridGameInstance->printPopulation();
    DEBUGPRINT("=====Printing initial population of grid2 after initializing gridGameInstance\n");
    //gridGameInstance2->printPopulation();

    DEBUGPRINT("=====Unregister Robot\n");
    gridGameInstance->unregisterRobot(1);
    //gridGameInstance->printPopulation();

    DEBUGPRINT("=====Get Sensor Data\n");
    int totalSensed = 0;
    gridGameInstance->returnSensorData(robots, sensed_items, totalSensed);

    // TEST for getRobots
    int teamid;
    float team_x;
    float team_y;
    std::vector<Msg_InitRobot>* les_robots = new std::vector<Msg_InitRobot>();

    Msg_TeamInit team_to_get;

    gridGameInstance->getRobots(team_to_get , les_robots);
    DEBUGPRINT("=====getRobots====\n");
    DEBUGPRINT("Should be depleted false: %d\n", (int)gridGameInstance->robotsDepleted());
    DEBUGPRINT("teamid:%d, teamx:%f, teamy:%f\n", teamid, team_x, team_y);
    /*
    for(std::vector<Msg_InitRobot>::iterator it = les_robots->begin(); it != les_robots->end(); it++)
    {
        DEBUGPRINT("id:%d,x:%f,y:%f\n", it->id, it->x, it->y);
    }
    */
    gridGameInstance->getRobots(team_to_get, les_robots);
    DEBUGPRINT("=====getRobots====\n");
    DEBUGPRINT("Should be depleted true: %d\n", (int)gridGameInstance->robotsDepleted());
    DEBUGPRINT("teamid:%d, teamx:%f, teamy:%f\n", teamid, team_x, team_y);
    /*
    for(std::vector<Msg_InitRobot>::iterator it = les_robots->begin(); it != les_robots->end(); it++)
    {
        DEBUGPRINT("id:%d,x:%f,y:%f\n", it->id, it->x, it->y);
    }
    */

    DEBUGPRINT("=====Printing population after getting all of the robots for robotclient\n");
    //gridGameInstance->printPopulation();

    DEBUGPRINT("=====Register Robot\n");
    gridGameInstance->registerRobot(newrobot);
    //gridGameInstance->printPopulation();


    DEBUGPRINT("=====Process action and update robots\n");
    Msg_Action process_robot;
    process_robot.robotid = 14;
    std::vector<Msg_Action> robotss;
    robotss.push_back(process_robot);
    std::vector<Msg_RobotInfo>* results = new std::vector<Msg_RobotInfo>;
    std::vector<std::pair<int, std::vector<Msg_RobotInfo> > >* robots_to_pass = new std::vector<std::pair<int, std::vector<Msg_RobotInfo> > >;
    int i = 0;
    while(i < 200)
    {
	    gridGameInstance->processAction(robotss, results, robots_to_pass);
        DEBUGPRINT("robots_to_pass.size %zu\n",
                    robots_to_pass->size());
        RobotInfoList* lhs = &robots_to_pass->at(0).second;
        RobotInfoList* rhs = &robots_to_pass->at(1).second;
        DEBUGPRINT("Number of elements l: %zu, r: %zu\n", lhs->size(), rhs->size());
        for(std::vector<std::pair<int, std::vector<Msg_RobotInfo> > >::iterator it = robots_to_pass->begin(); it != robots_to_pass->end(); it++)
        {
            std::pair<int, RobotInfoList > some_pair = (*it);
            RobotInfoList update_robots = some_pair.second;
            if(update_robots.size() > 0)
            {
                gridGameInstance2->updateRobots(some_pair.second);
            }
        }
        DEBUGPRINT("=====Printing population for grid 1\n");
	    gridGameInstance->printPopulation();
        DEBUGPRINT("=====Printing population for grid 2\n");
	    gridGameInstance2->printPopulation();
        i++;
    }

    #else
	gridGameInstance = new GridGame(m_uId, m_teamsAvailable, m_robotsPerTeam,
	    m_idRangeFrom, m_idRangeTo, m_homeRadius, m_worldSize, m_numGrids, m_puckTotal);
    #endif

	return 0;
}

int GridServer::initClock(const char* host, const char* ip)
{
    int l_ClockFd = initConnection(host,ip);

    if (l_ClockFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
		return -1;
    }

    m_ClockFd = l_ClockFd;

    return l_ClockFd;
}

int GridServer::handleNewConnection(int fd)
{
    return 0;
}

int GridServer::allConnectionReadyHandler()
{
    return 0;
}

void * drawer_function(void* gridPtr)
{
    Network::GridServer * grid = (Network::GridServer *)gridPtr;
    uint32_t objsUpdated = 0;

    for(uint32_t frame = 0; true; frame++)
    {
        usleep(FRAME_FREQUENCY);

        try
        {
        	objsUpdated = grid->updateDrawer(frame);
        	if (objsUpdated > 0)
            DEBUGPRINT("UpdateDrawer return[%d]: %d\n", frame, objsUpdated);
        }
        catch(std::exception & e)
        {
            ERRORPRINT("Failed To Updated Drawer.\n");
            perror(e.what());
            return NULL;
        }
    }

    DEBUGPRINT("Exitting drawer_function()\n");   
	return NULL;
}


int GridServer::handler(int fd)
{
	TcpConnection * l_curConnection = m_Clients[fd];
	
	if (l_curConnection == NULL)
    {
		return -1; // no such socket descriptor
	}
	
	uint16_t l_sender=-1, l_senderMsg =-1; // sanity check
	
	if (NetworkCommon::recvHeader(l_sender, l_senderMsg, l_curConnection) < 0)
	{
		ERRORPRINT("GRID_SERVER ERROR:\t Failed to get header\n");
		return -1;
	} 

	DEBUGPRINT("GRID_SERVER STATUS:\t Handeling message from sender=%u, message=%u\n", l_sender, l_senderMsg);
	
	if (l_sender == -1 || l_senderMsg == -1) 
	{
		ERRORPRINT("GRID_SERVER ERROR:\t Either the sender or message returned -1\n");
		return -1; // bad messages
	}
	
	switch (l_sender) // check the sender
	{
        case SENDER_CLOCK:
        {
            switch(l_senderMsg)
            {
                case(MSG_HEARTBEAT) :
                {
                   Msg_HB l_HB;
                   unsigned char *l_hbBuffer = new unsigned char[l_HB.size]; 

                   if (l_hbBuffer == NULL)
                   {
                       ERRORPRINT("Couldn't allocate buffer space for receiving heartbeat\n");
                       return -1;
                   }
                  
                   //Receive the heartbeat.
                   NetworkCommon::recvWrapper(l_curConnection, l_hbBuffer, l_HB.size); 

                   //Unpack heartbeat message from our buffer.
                   unpack(l_hbBuffer, Msg_HB_format, &l_HB.hb);

                   m_Hb = l_HB.hb; 

                   delete []l_hbBuffer;

                   return 0;
                }
                default:
                {
                    return -1;
                }
            }
            
        }
        case SENDER_GRIDSERVER:
        {
            switch(l_senderMsg) // check the message
            {
                case(MSG_BOUNDARYUPDATE):
                {
                    Msg_header l_Header;
                    Msg_MsgSize l_Size;
              
                    unsigned char l_SizeBuffer[l_Size.size];

                    if (l_curConnection->recv(l_SizeBuffer, l_Size.size) == -1)
				    {
							ERRORPRINT("GRID_SERVER ERROR:\t Couldn't receive the number of boundary robo updates.\n");
							return -1;
					}
					
                    unpack(l_SizeBuffer, Msg_MsgSize_format, &l_Size.msgSize);

                    DEBUGPRINT("Received grid boundary robo information for %hd robos\n", l_Size.msgSize);

                    Msg_RobotInfo l_RoboInfo;
                    std::vector<Msg_RobotInfo> l_RoboUpdates;
               
                    int l_MessageSize = l_RoboInfo.size*l_Size.msgSize; 
                    
                    unsigned char * l_Buffer = new unsigned char [l_MessageSize];
                    
                    if (l_Buffer == NULL)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR\t Failed to allocate memory for message\n");
                    	return -1;
                    }
                    
                    if (l_curConnection->recv(l_Buffer, l_MessageSize) == -1)
				    {
							ERRORPRINT("Couldn't receive the number of boundary robo updates.\n");
							return -1;
					}

                    int l_Offset = 0;

                    for (int i = 0; i < l_Size.msgSize; i++)
                    {
                        unpack(l_Buffer+l_Offset, Msg_RobotInfo_format, &l_RoboInfo.robotid, &l_RoboInfo.x_pos, &l_RoboInfo.y_pos, &l_RoboInfo.speed, &l_RoboInfo.angle, &l_RoboInfo.puckid, &l_RoboInfo.gridid);
                        l_RoboUpdates.push_back(l_RoboInfo);
                        DEBUGPRINT("Received boundary robo with %d %f %f\n", l_RoboInfo.robotid, l_RoboInfo.x_pos, l_RoboInfo.y_pos);
                        l_Offset += l_RoboInfo.size;
                    }

                    if (gridGameInstance->updateRobots(l_RoboUpdates) < 0)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to update robots inside GRID_GAME\n");
                    	return -1;
                    }

                    m_ReadyPartners++;
                    if (m_ReadyPartners == NUM_NEIGHBOURS*m_numClients)
                    {
                        gridGameInstance->sortPopulation();
                        Msg_header l_Header = {SENDER_CLIENT, MSG_HEARTBEAT};
                        Msg_HB l_HB = {m_Hb};

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

                        TcpConnection *l_ClockConn = m_Clients[m_ClockFd];
                        NetworkCommon::sendWrapper(l_ClockConn, l_HBBuffer, l_MessageSize);
                        DEBUGPRINT("Sent heartbeat. %hd\n", m_Hb);
                        m_ReadyPartners = 0;

                        delete[]l_HBBuffer;
                    }
					
					// Clean up all allocated memory
					delete []l_Buffer;
					
                return 0;
                }
            }
        }
        break;
        case SENDER_CONTROLLER:
        {
            switch(l_senderMsg)
            {
                case(MSG_GRIDNEIGHBOURS):
                {
                	LOGPRINT("GRID_SERVER STATUS:\t Getting neigbour information from grid\n");
                    Msg_MsgSize l_NumNeighbours;                                      
                    unsigned char l_SizeBuffer[l_NumNeighbours.size];

                    //Receive and unpack the number of neighbours.
                    if (l_curConnection->recv(l_SizeBuffer, l_NumNeighbours.size) < 0)
				    {
							ERRORPRINT("GRID_SERVER ERROR:\t Couldn't receive the number of neighbour.\n");
							return -1;
					}
					
                    unpack(l_SizeBuffer, Msg_MsgSize_format, &l_NumNeighbours.msgSize);

                    DEBUGPRINT("GRID_SERVER STATUS:\t Received grid neighbour information for %hd neighbours\n", l_NumNeighbours.msgSize);

                    Msg_GridNeighbour l_Neighbour;
                    //Create a buffer big enough to hold all of the expected neighbours.
                    unsigned int l_SizeOfNeighbourInfo = l_Neighbour.size*l_NumNeighbours.msgSize;
                    unsigned char * l_NeighbourBuffer = new unsigned char[l_SizeOfNeighbourInfo];

					if (l_NeighbourBuffer == NULL)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to allocate memory in Grid Neighbours\n");
						return -1;
					}
		
                    if (l_curConnection->recv(l_NeighbourBuffer, l_SizeOfNeighbourInfo) == -1)
				    {
							ERRORPRINT("Couldn't receive the grid neighbours.\n");
							return -1;
					}                   
					
					unsigned int l_Offset = 0;

                    for (int i = 0; i < l_NumNeighbours.msgSize;i++)
                    {
                        unpack(l_NeighbourBuffer+l_Offset, Msg_gridNeighbour_format, &l_Neighbour.position,
                                &l_Neighbour.ip, &l_Neighbour.port);
                        l_Offset += l_Neighbour.size;
                        DEBUGPRINT("Received a new neighbour at position %hd, with IP %s and Port %s\n",
                                l_Neighbour.position, l_Neighbour.ip, l_Neighbour.port);
                        m_GridPosToFd[l_Neighbour.position] = initConnection(l_Neighbour.ip, l_Neighbour.port); 
                        if (m_GridPosToFd[l_Neighbour.position] < 0)
                        {
                        	ERRORPRINT("GRID_SERVER STATUS:\t Bad Socket Descriptor\n");
                        	return -1;
                        }
                        //TODO Handle new neighbour.
                    }
                    
        			delete []l_NeighbourBuffer;            
		            break;
				}
				case (MSG_REQUESTGRIDWAITING):
				{
					m_ControllerFd = fd;
					
					LOGPRINT("GRID_SERVER STATUS: Controller asked if this grid is ready\n");
					Msg_GridId l_gridId;
					unsigned char l_gridIdBuff[l_gridId.size];
					
					Msg_header l_header;
					
					Msg_WorldInfo l_worldInfo;
					unsigned char l_worldInfoBuff[l_worldInfo.size];
					
					
					if (l_curConnection->recv(l_gridIdBuff, l_gridId.size) < 0)
					{
						ERRORPRINT("GRID_SERVER FAILED:\t failed to receive id from controller.\n");
						return -1;
					}
					
					if (l_curConnection->recv(l_worldInfoBuff, l_worldInfo.size) < 0) 
					{
						ERRORPRINT("GRID_SERVER FAILED:\t Failed to receive world information\n");
						return -1;
					}
					
					unpack(l_gridIdBuff, Msg_GridId_format, &m_uId); // unpack this grid's id
					unpack(l_worldInfoBuff, Msg_WorldInfo_format, &m_homeRadius, &m_worldSize, &m_numGrids, &m_puckTotal); // unpack all world info

					DEBUGPRINT("World info Home Radius=%f World Size=%f Number of Grids=%i Puck Total=%i \n", m_homeRadius, m_worldSize, m_numGrids, m_puckTotal);

					// Now grid has it's id, and world info it needs to know about the range 

					Msg_GridRequestIdRage l_reqGridRange;
										
					unsigned char * l_gridBuff = new unsigned char[l_header.size + l_gridId.size + l_reqGridRange.size];
					
					if (l_gridBuff == NULL)
					{
						ERRORPRINT("GRID_SERVER ERROR\t Failed to allocate memory in request grid waiting\n");
						return -1;
					}
					
					if (NetworkCommon::packHeader(l_gridBuff, SENDER_GRIDSERVER, MSG_RESPONDGRIDWAITING) < 0)
					{
						ERRORPRINT("GRID_SERVER FAILED:\t Cannot pack ready header to controller.\n");
						return -1;
					}
					
					if (pack(l_gridBuff+l_header.size, "lll", m_uId, (unsigned long)m_teamsAvailable, (unsigned long)m_robotsPerTeam) != (l_gridId.size + l_reqGridRange.size))
					{
						ERRORPRINT("GRID_SERVER FAILED:\t Cannot send grid waiting header to controller\n");
						return -1;
					}
					
					uint32_t id;

					if (NetworkCommon::sendMsg(l_gridBuff, l_header.size+l_gridId.size+l_reqGridRange.size, l_curConnection) < 0)
					{
						ERRORPRINT("GRID_SERVER FAILED:\t Cannot send grid waiting header to controller\n");
						return -1;
					}
					
					LOGPRINT("GRID_SERVER STATUS:\t Sending Grid Waiting message to controller\n");

					delete []l_gridBuff;					
					break;
				}
				case (MSG_RESPONDGRIDRANGE):
				{
					LOGPRINT("GRID_SERVER STATUS:\t Receiving Range Information\n");
					
					Msg_RobotIdRange l_range;
					unsigned char l_rangeBuff[l_range.size];
					
					if (l_curConnection->recv(l_rangeBuff, l_range.size) < 0)
					{
						DEBUGPRINT("GRID_SERVER FAILED:\t could not receive grid range data\n");
						return -1;
					}
					
					unpack(l_rangeBuff, Msg_RobotIdRange_format, &m_idRangeFrom, &m_idRangeTo);
					
					DEBUGPRINT("GRID_SERVER STATUS:\t Id Range from=%lu to=%lu\n", (unsigned long)m_idRangeFrom, (unsigned long)m_idRangeTo);
				
					m_robotsAvailable = m_idRangeTo - m_idRangeFrom;
					
					DEBUGPRINT("GRID_SERVER STATUS:\t TOTAL ROBOTS %lu\n", (unsigned long)m_robotsAvailable);
					
					initGridGame();
					
					break;
				}
            }
        }
        break;
        case SENDER_CLIENT:
			
            switch(l_senderMsg)
            {
				case (MSG_REQUESTSENSORDATA):
				{
					DEBUGPRINT("GRID_SERVER STATUS:\t Receiving request for sensory information\n");
					Msg_MsgSize l_msgSize;
					
					if (NetworkCommon::recvMessageSize(l_msgSize, l_curConnection) < 0)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to receive message size\n");
						return -1;
					}
					
					int l_numRobots = l_msgSize.msgSize;
					
					if (l_numRobots < 0)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Requested sensor data for less than or equal to zero robots.\n");
						return -1;
					}
					
					DEBUGPRINT("GRID_SERVER STATUS:\tClient is requesting sensory data for %i robots\n", l_numRobots);
					
					std::vector<uid> l_robotIdVector;
					l_robotIdVector.clear();
					
					Msg_RobotId l_robotId;
					
					unsigned char * l_robotIdBuff = new unsigned char[l_robotId.size * l_numRobots];
					
					if (l_robotIdBuff == NULL)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to allocate memory for sensory request\n");
						return -1;
					}
					
					for (int i = 0; i < l_numRobots; i++)
					{
						if (l_curConnection->recv(l_robotIdBuff+(l_robotId.size * i), l_robotId.size) == -1)
						{
							ERRORPRINT("GRID_SERVER ERROR:\t Could not get robot id for returnSensorData\n");
							return -1;
						}
						
						unsigned int id;
						unpack(l_robotIdBuff+(l_robotId.size * i),Msg_RobotId_format, &id);
						l_robotIdVector.push_back(id);
						DEBUGPRINT("GRID_SERVER STATUS:\t Pushed robot with ID=%i\n", id);
					}

					int l_robotsTotal = l_robotIdVector.size(); // how many robots have sensory data
					int l_totalSensed = 0; // total of all the robots sensory data *how many sensed items in total*
					
					// has map that contains a list for each robot in request list
					std::vector< RobotSensedObjectsPair >* sensedItems = new std::vector< RobotSensedObjectsPair >(); 
                    
                    // collect the sensory information
					if(gridGameInstance->returnSensorData(l_robotIdVector, sensedItems, l_totalSensed) < 0)
					{
					    ERRORPRINT("FAILED TO GET ALL SENSOR DATA. Exiting.\n");
						return -1;
					} 
					
					DEBUGPRINT("GRID_SERVER STATUS:\t Got all robot sensory data. Total of %d robots with %d sensory objects\n",
					            l_robotsTotal, l_totalSensed);
					
					Msg_header l_header = {SENDER_GRIDSERVER, MSG_RESPONDSENSORDATA}; // header for response
					memset(&l_msgSize, 0 , l_msgSize.size);
					
					l_msgSize.msgSize = l_robotsTotal;
					
					Msg_SensedObjectGroupHeader l_robotHeader; // for each robot
					Msg_SensedObjectGroupItem l_sensedObject; // for each sensed item
					
					unsigned int l_responseMsgSize = 0;
					
					l_responseMsgSize += l_header.size; // append header size
					l_responseMsgSize += l_msgSize.size; // append size of msg length 
					l_responseMsgSize += (l_robotsTotal * l_robotHeader.size); // total number or robots
					l_responseMsgSize += (l_totalSensed * l_sensedObject.size); // total number of sensed items
					
					DEBUGPRINT("GRID_SERVER STATUS:\t Packing Message size of %i\n", l_responseMsgSize);
					
					unsigned char* l_msgBuffer = new unsigned char[l_responseMsgSize];
					
					if (l_msgBuffer == NULL)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to allocate memory for sensory response\n");
						return -1;
					}
					
					if (pack(l_msgBuffer, Msg_header_format, l_header.sender, l_header.message) != l_header.size)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Could not pack header\n");
						return -1;
					}
					
					int l_position = l_header.size;
					
					if (pack(l_msgBuffer+l_position, Msg_MsgSize_format, l_msgSize.msgSize) != l_msgSize.size) // pack number of robots
					{
						ERRORPRINT("GRID_SERVER ERROR:\tCould not pack number of robots\n");
						return -1;
					}
					
					l_position += l_msgSize.size; // shift by robot size header
					
					DEBUGPRINT("GRID_SERVER STATUS:\tPacked Message size of %u\n", l_position);
					
					std::vector< RobotSensedObjectsPair >::iterator mapEnd = sensedItems->end(); 
					 // sensed item iterator
					
                    Msg_SensedObjectGroupItem l_sensedItem;								
					for(std::vector< RobotSensedObjectsPair >::iterator it = sensedItems->begin(); it != mapEnd; it++)
					{
						// for each robot requested
						l_robotHeader.id = it->first;
						l_robotHeader.objectCount = it->second.size();
						DEBUGPRINT("GRID_SERVER STATUS:\t Packing robot %d with %d sensory objects\n", l_robotHeader.id, l_robotHeader.objectCount);
						if (pack(l_msgBuffer+l_position, Msg_SensedObjectGroupHeader_format, l_robotHeader.id, l_robotHeader.objectCount) != l_robotHeader.size)
						{
							ERRORPRINT("GRID_SERVER ERROR:\t Could not pack robot header\n");
							return -1;
						}
						
						l_position += l_robotHeader.size;
						DEBUGPRINT("GRID_SERVER STATUS:\t Packed Message size of %u\n", l_position);
						
						std::vector<Msg_SensedObjectGroupItem>::iterator sensedItemEnd = it->second.end(); 
						
						for (std::vector<Msg_SensedObjectGroupItem>::iterator vecIt = it->second.begin();
						     vecIt != sensedItemEnd;
						     vecIt++)
						{
						    Msg_SensedObjectGroupItem l_sensedItem = (*vecIt);		
							DEBUGPRINT("GRID_SERVER STATUS:\t Packing sensed object id=%d x=%d y=%d at buffer size of %d\n",
							             l_sensedItem.id, l_sensedItem.x, l_sensedItem.y, l_position);
							             
							if (pack(l_msgBuffer+l_position,
							         Msg_SensedObjectGroupItem_format,
							         l_sensedItem.id,
							         l_sensedItem.x,
							         l_sensedItem.y) != l_sensedItem.size)
							{
								ERRORPRINT("GRID_SERVER STATUS:\t Could not pack robot header\n");
								return -1;
							}
							
							l_position += l_sensedItem.size;
						}
						
					}
					DEBUGPRINT("GRID_SERVER STATUS:\t Position in Buffer: %d, Size of message: %d", l_position, l_responseMsgSize);
					
					if (l_curConnection->send(l_msgBuffer, l_responseMsgSize) < 0)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to send sensory data\n");
						return -1;
					}
					
					DEBUGPRINT("GRID_SERVER STATUS:\t Sent sensory response to grid\n");

					sensedItems->clear();
					delete sensedItems;
                    delete []l_msgBuffer;
                    delete []l_robotIdBuff;
						
					break;
					
				} // end of message 
				
				case (MSG_PROCESSACTION):
				{
                    Msg_header l_Header;
                    Msg_MsgSize l_Size;
                    Msg_Action l_Action;

                    if (NetworkCommon::recvMessageSize(l_Size, l_curConnection) < 0)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to receive message size\n");
                    	return -1;
                    }

                    unsigned int l_MessageSize = l_Size.msgSize*l_Action.size;
                    
                    unsigned char * l_ActionBuffer = new unsigned char[l_MessageSize];
                    
                    if (l_ActionBuffer == NULL)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to allocate memory for process action\n");
                    	return -1;
                    }
                    
                  	if (l_curConnection->recv(l_ActionBuffer, l_MessageSize) < 0)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Could not get Msg_Actions for processs actions\n");
						return -1;
					}

                    std::vector<Msg_Action> l_Actions;
                    
                    unsigned int l_Offset = 0;
                    
                    DEBUGPRINT("GRID_SERVER STATUS:\t Received a total of %d action requests\n", l_Size.msgSize);
                    
                    for (int i = 0; i < l_Size.msgSize; i++)
                    {
                        unpack(l_ActionBuffer+l_Offset, Msg_Action_format, &l_Action.robotid, &l_Action.action, 
                                &l_Action.speed, &l_Action.angle);
                        l_Actions.push_back(l_Action);
                        l_Offset += l_Action.size;
                    }

                    std::vector<Msg_RobotInfo> l_Results;

                    
                    std::vector<std::pair<int, std::vector<Msg_RobotInfo> > > robots_to_pass;
                    
                    if (gridGameInstance->processAction(l_Actions, &l_Results, &robots_to_pass) < 0)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to process actions inside of GRID_GAME\n");
                    	return -1;
                    }

                    DEBUGPRINT("GRID_SERVER STATUS:\t Received %zu results\n", l_Results.size());

                    Msg_RobotInfo l_Result;
                    l_MessageSize = l_Header.size+l_Size.size+(l_Result.size*l_Size.msgSize);

                    l_Offset = 0;
                    
                    unsigned char * l_ResultsBuffer = new unsigned char[l_MessageSize];
                    
                    if (l_ResultsBuffer == NULL)
                    {
                    	ERRORPRINT("GRID_SERVER STATUS:\t Failed to allocate memory in process action\n");
                    	return -1;
                    }
                    
                    if (NetworkCommon::packHeader(l_ResultsBuffer+l_Offset, SENDER_GRIDSERVER,MSG_RESPONDPROCESSACTION) < 0)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to pack header\n");
                    	return -1;
                    }
                    
                    l_Offset += l_Header.size;

                    if (pack(l_ResultsBuffer+l_Offset, Msg_MsgSize_format, l_Size.msgSize) != l_Size.size)
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Fialed to pack number of robot sensory responses\n");
                    	return -1;
                    }
                    
                    l_Offset += l_Size.size;

                    DEBUGPRINT("GRID_SERVER ERROR:\t Prepairing to pack %d\n", l_Size.msgSize);
                    
                    for (int i = 0; i < l_Size.msgSize; i++)
                    {
                        if (pack(l_ResultsBuffer+l_Offset, Msg_RobotInfo_format, l_Results[i].robotid, l_Results[i].x_pos, l_Results[i].y_pos, 
                        		 l_Results[i].speed, l_Results[i].angle, l_Results[i].puckid, l_Results[i].gridid)
                        		 != l_Result.size
                        	)
                        	{
								ERRORPRINT("GRID_SERVER ERROR\t Failed to pack a action response\n");                        	
                        		return -1;
                        	}
                        	
                        l_Offset += l_Result.size;
                    }

                    if (NetworkCommon::sendMsg(l_ResultsBuffer, l_MessageSize, l_curConnection) < 0 )
                    {
                    	ERRORPRINT("GRID_SERVER ERROR:\t Failed to send message\n");
                    	return -1;
                    }

                    l_Header.sender = SENDER_GRIDSERVER;
                    l_Header.message = MSG_BOUNDARYUPDATE;

                    Msg_RobotInfo l_RoboInfo;
                    
                    int l_BoundarySize = robots_to_pass.size();
                    
                    
                    for(int i = 0; i < l_BoundarySize; i++)
                    {
                        l_Offset = 0;

                        DEBUGPRINT("GRID_SERVER STATUS:\t Attempting to send to grid at position %d with fd %d\n", robots_to_pass[i].first, m_GridPosToFd[robots_to_pass[i].first]);
                        
                        TcpConnection* l_GridCon = m_Clients[m_GridPosToFd[robots_to_pass[i].first]];
                        
                        if (l_GridCon == NULL)
                        {
                        	ERRORPRINT("GRID_SERVER ERROR:\t No valid connection\n");
                        	return -1;
                        }
                        
                        l_Size.msgSize = robots_to_pass[i].second.size();

                        DEBUGPRINT("GRID_SERVER STATUS:\t Attempting to send %d boundary robo infos to another grid\n", l_Size.msgSize);
                        
                        l_MessageSize = l_Header.size+l_Size.size+(l_Size.msgSize * l_RoboInfo.size);

                        unsigned char * l_BoundaryBuffer = new unsigned char[l_MessageSize];
                        
                        if (l_BoundaryBuffer == NULL)
                        {
                        	ERRORPRINT("GRID_SERVER ERROR:\t Failed to allocate memory for boundary information\n");
                        	return -1;
                        }
                        
                        if (NetworkCommon::packHeader(l_BoundaryBuffer+l_Offset, SENDER_GRIDSERVER, MSG_BOUNDARYUPDATE) < 0)
                        {
                        	ERRORPRINT("GRID_SERVER ERROR:\t Failed to pack header\n");
                        	return -1;
                        }
                        
                        l_Offset += l_Header.size;

                        if (pack(l_BoundaryBuffer+l_Offset, Msg_MsgSize_format, l_Size.msgSize) != l_Size.size)
                        {
                        	ERRORPRINT("GRID_SERVER ERROR\t Failed to pack header\n");
                        	return -1;
                        }
                        
                        l_Offset += l_Size.size;

                        for (int a = 0; a < l_Size.msgSize; a++)
                        {
                            Msg_RobotInfo& l_RoboToPack = robots_to_pass[i].second[a];
                            DEBUGPRINT("GRID_SERVER STATUS:\t Boundary robo with id %d and %f %f\n", l_RoboToPack.robotid, robots_to_pass[i].second[a].x_pos, robots_to_pass[i].second[a].y_pos);
                            if (pack(l_BoundaryBuffer+l_Offset, Msg_RobotInfo_format, l_RoboToPack.robotid, l_RoboToPack.x_pos, l_RoboToPack.y_pos, 
                            	 l_RoboToPack.speed, l_RoboToPack.angle, l_RoboToPack.puckid, l_RoboToPack.gridid)
                            	 != l_RoboToPack.size
                            )
                            {
                            	ERRORPRINT("GRID_SERVER ERROR:\t Failed to pack robot information\n");
                            	return -1;
                            }
                            l_Offset += l_RoboToPack.size;
                        }
                        
						DEBUGPRINT("GRID_SERVER STATUS:\t Prepairing to send\n");
						
                        if (NetworkCommon::sendMsg(l_BoundaryBuffer, l_MessageSize, l_GridCon) < 0)
                        {
                        	ERRORPRINT("GRID_SERVER ERROR\t Failed to send message\n");
                        	return -1;
                        }
                        
                        DEBUGPRINT("GRID_SERVER STATUS:\t Sent Message\n");
                        
						delete []l_BoundaryBuffer;
						
                    }				
					
					delete []l_ActionBuffer;
					delete []l_ResultsBuffer;
								
		            break;
				}
				
				case (MSG_REQUESTINITTEAM):
				{
					LOGPRINT("GRID_SERVER STATUS:\t Requesting to initialize a team\n");
					
					Msg_TeamInit l_Team;
					std::vector<Msg_InitRobot> * l_robots = new std::vector<Msg_InitRobot>;
					
					if (l_robots == NULL)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to allocate memory\n");
						return -1;
					}
					
					if (gridGameInstance->getRobots(l_Team, l_robots) < 0)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to get robots in GRID_GAME\n");
						return -1;
					}

					DEBUGPRINT("GRID_SERVER STATUS:\t Prepairing init to for %zu robots\n", l_robots->size());
					
					Msg_header l_header;
					Msg_MsgSize l_size;
					Msg_InitRobot l_robot;
					
					size_t l_length = l_header.size + l_Team.size + l_size.size + (l_robots->size() * l_robot.size);
					
					unsigned char * l_message = new unsigned char[l_length];
					
					if (l_message == NULL)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to allocate memory for GRID_SERVER\n");
						return -1;
					}
					
					
					int l_offset = 0;
					
					if (NetworkCommon::packHeader(l_message, SENDER_GRIDSERVER, MSG_RESPONDINITTEAM) < 0)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to pack header\n");
						return -1;
					}
					
					l_offset += l_header.size;

					if (pack(l_message+l_offset, Msg_TeamInit_format, l_Team.id, l_Team.x, l_Team.y) != l_Team.size)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to pack message: Team information\n");
						return -1;
					}

					LOGPRINT("GRID_SERVER STATUS\t Starting team ID:%i, Position X:%f Y:%f, with %u robots\n", l_robots->size());

					l_offset += l_Team.size;

					l_size.msgSize = l_robots->size();
					
					if (pack(l_message+l_offset, Msg_MsgSize_format, l_size.msgSize) != l_size.size)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to send amount of robots\n");
						return -1;
					}
					
					l_offset += l_size.size;

					for( int i = 0; i < m_robotsPerTeam;i ++ )
					{
						l_robot.id = l_robots->at(i).id;
						l_robot.x = l_robots->at(i).x;
						l_robot.y = l_robots->at(i).y;
						
						DEBUGPRINT("GRID_SERVER STATUS:\t Going to send robot with ID: %d, X: %f, Y: %f\n", l_robot.id, l_robot.x, l_robot.y);
						
						if (pack(l_message+l_offset, Msg_InitRobot_format, l_robot.id, l_robot.x, l_robot.y) != l_robot.size)
						{
							ERRORPRINT("GRID_SERVER ERROR:\t Failed to pack robot information\n");
							return -1;
						}
						
						l_offset += l_robot.size;
					}
					
					if (NetworkCommon::sendMsg(l_message, l_length, l_curConnection) < 0)
					{
						ERRORPRINT("GRID_SERVER ERROR:\t Failed to send message\n");
						return -1;
					}

					l_robots->clear();
					delete l_robots;
					delete []l_message;
		            return 0;
				}
				
				case (MSG_CONFIRMTEAM):
				{
					LOGPRINT("GRID_SERVER STATUS: Client Responded with team CONFIRMATION\n");
					m_teamsConfirmed++;
					LOGPRINT("GRID_SERVER STATUS: Total of %i teams have been confirmed\n", m_teamsConfirmed);
					
					if (m_teamsConfirmed == m_teamsAvailable)
					{
						TcpConnection * contCon = m_Clients[m_ControllerFd];
						if (contCon == NULL)
						{
							ERRORPRINT("GRID_SERVER ERROR:\t Connection does not exist\n");
							return -1;
						}
						Msg_header l_header;
						Msg_GridId l_msg;
						
						unsigned char message[l_header.size + l_msg.size];
						
						if (NetworkCommon::packHeader(message, SENDER_GRIDSERVER,MSG_GRIDCONFIRMSTARTED) < 0)
						{
							ERRORPRINT("GRID_SERVER FAILED:\t Failed to pack header confirm started\n");
							return -1;
						}
						
						if (pack(message+l_header.size, Msg_GridId_format, m_uId) != l_msg.size)
						{
							ERRORPRINT("GRID_SERVER FAILED:\t Failed to pack id for confirm started\n");
							return -1;
						}
						
						if (NetworkCommon::sendMsg(message, l_header.size+l_msg.size, contCon) < 0)
						{
							ERRORPRINT("GRID_SERVER FAILED:\t Failed to send confirm started message\n");
							return -1;
						}
						
						LOGPRINT("GRID_SERVER STATUS:\t Sent controller CONFIRMATION\n");
					}				
					
					return 0;
				}
				
				default:
				{
					ERRORPRINT("GRID_SERVER ERROR\t: No matching message handle for client case.\n");
					return -1;
				}
			} 
		break;
		case SENDER_DRAWER:
            switch(l_senderMsg)
            {
                case(MSG_SETDRAWERCONFIG):
                {
                    //DEBUGPRINT("Recieved Drawer Instruction\n");

                    Msg_DrawerConfig configData;
                    unsigned char * configDataBuf = new unsigned char[configData.size];

                    if (l_curConnection->recv(configDataBuf, configData.size) == -1)
					{
						DEBUGPRINT("Could not receive a config data.");
						return -1;
					}
					unpack(configDataBuf, Msg_DrawerConfig_format, &configData.send_data, &configData.data_type, &configData.tl_x, &configData.tl_y, &configData.br_x, &configData.br_y);
					delete configDataBuf;

                    //DEBUGPRINT("Config: send_data=%c, data_type=%c, tl_x=%f, tl_y=%f, br_x=%f, br_y=%f\n",
                           //configData.send_data, configData.data_type, configData.tl_x, configData.tl_y, configData.br_x, configData.br_y);

					// If this is the first message from the Drawer, send team home data and create update thread
					if (!m_drawerConn)
					{
						//Send team data
						std::vector<Msg_TeamInit> objects;
						gridGameInstance->getTeams(&objects);
						
						Msg_header l_header = {SENDER_GRIDSERVER, MSG_GRIDTEAMS}; // header for response
						Msg_MsgSize l_msgSize;	
						Msg_TeamInit l_TeamInfo;
						l_msgSize.msgSize = objects.size(); //number of teams with homes on grid

						unsigned int l_responseMsgSize = 0;	
						l_responseMsgSize += l_header.size; 					// append header size	
						l_responseMsgSize += l_msgSize.size;					// append msgSize size
						l_responseMsgSize += (l_msgSize.msgSize * l_TeamInfo.size); // append the total size for number of team info

						unsigned char * msgBuffer = new unsigned char[l_responseMsgSize];

						if (pack(msgBuffer, Msg_header_format, l_header.sender, l_header.message) != l_header.size)
						{
							DEBUGPRINT("Could not pack header\n");
							return -1;
						}

						int l_position = l_header.size;

						if (pack(msgBuffer+l_position, Msg_MsgSize_format, l_msgSize.msgSize) != l_msgSize.size) // pack number of robots
						{
							DEBUGPRINT("Could not pack number of robots\n");
							return -1;
						}

						l_position += l_msgSize.size; // shift by robot size header
	
						for(std::vector<Msg_TeamInit>::iterator it = objects.begin(); it != objects.end(); it++)
						{										   
							if (pack(msgBuffer+l_position, Msg_TeamInit_format,
									 (*it).id, (*it).x, (*it).y ) != l_TeamInfo.size)
							{
								DEBUGPRINT("Could not pack robot header\n");
								return -1;
							}

							l_position += l_TeamInfo.size;
						}

						if(l_curConnection->send(msgBuffer, l_responseMsgSize) == -1)
						{
							DEBUGPRINT("Failed to send\n");
						}
						delete msgBuffer;
						DEBUGPRINT("Sent %d teams.\n", l_msgSize.msgSize);
						
                        m_drawerConn = l_curConnection;

						//create update thread
		                pthread_t update_thread;
		                int iret1 = pthread_create(&update_thread, NULL, drawer_function, (void *)this);
		                if(iret1 != 0)
		                {
		                    perror("pthread failed");
		                    return -1;
		                }
                    }
                    
                    //change send-to-drawer status accordingly
                    float l_gridLeft = gridGameInstance->getLeftBoundary();
                    float l_gridRight = gridGameInstance->getRightBoundary();
                    m_updateDrawerFlag = !(l_gridLeft > configData.br_x || l_gridRight < configData.tl_x);
                    m_drawerTop = configData.tl_y;
                    m_drawerBottom = configData.br_y;
                    m_drawerLeft = configData.tl_x;
                    m_drawerRight = configData.br_x;
            		
                    return 0;
                }

                default:
                {
                    DEBUGPRINT("No matching message handle for drawer case.\n");
                    return -1;
                }
            }   		
		default:
		{
			printf("no matching msg handler for UNKNOWN\n");
			return -1;
		} // end of default
		
	} // end of switch

    return 0;
}

int GridServer::updateDrawer(uint32_t framestep)
{
	if (!m_updateDrawerFlag) return 0;

    Msg_header l_Header = {SENDER_GRIDSERVER, MSG_GRIDDATAFULL };


    Msg_MsgSize l_Size;
    Msg_RobotInfo l_RoboInfo;

    std::vector<Msg_RobotInfo> objects;
    gridGameInstance->getPopulation(&objects, m_drawerTop, m_drawerBottom, m_drawerLeft, m_drawerRight);

    l_Size.msgSize = objects.size();
    DEBUGPRINT("updateDrawer: Sending %d objects.\n", l_Size.msgSize);

    unsigned int l_Offset = 0;
    unsigned int l_MessageSize = l_Header.size + l_Size.size + (l_RoboInfo.size*l_Size.msgSize);
    unsigned char *l_Buffer = new unsigned char[l_MessageSize];

    NetworkCommon::packHeader(l_Buffer+l_Offset, SENDER_GRIDSERVER, MSG_GRIDDATAFULL);
    l_Offset += l_Header.size;

    pack(l_Buffer+l_Offset, Msg_MsgSize_format, l_Size.msgSize);
    l_Offset += l_Size.size;

    for (int i = 0; i < l_Size.msgSize; i++)
    {
        pack(l_Buffer+l_Offset, Msg_RobotInfo_format, objects[i].robotid, objects[i].x_pos, objects[i].y_pos, 0, 0, 0 ); 
        
        //unpack(l_Buffer+l_Offset, Msg_RobotInfo_format,
                                //&l_RoboInfo.robotid, &l_RoboInfo.x_pos, &l_RoboInfo.y_pos, &l_RoboInfo.speed, &l_RoboInfo.angle, &l_RoboInfo.puckid, &l_RoboInfo.gridid );
        //DEBUGPRINT("Object: id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%d\n",
                        	        //l_RoboInfo.robotid, l_RoboInfo.x_pos, l_RoboInfo.y_pos, l_RoboInfo.angle, l_RoboInfo.puckid );
                         
        l_Offset += l_RoboInfo.size;
    }

    if(!m_drawerConn || m_drawerConn->send(l_Buffer, l_MessageSize) == -1)
    {
        DEBUGPRINT("Failed to send\n");
        return -1;
    }   
    
    delete l_Buffer;
    return l_Size.msgSize;
}

void Network::GridServer::setTeams(int amount)
{
	m_teamsAvailable = amount;
}

void Network::GridServer::setIdRange(int from, int to)
{
	m_idRangeFrom = from;
	m_idRangeTo = to;
}

void Network::GridServer::setRobotsPerTeam(int amount)
{
	m_robotsPerTeam = amount;
}

void Network::GridServer::setId(int id)
{
	m_uId = id;
}

void Network::GridServer::setNumClients(int numClients)
{
	m_numClients = numClients;
}

