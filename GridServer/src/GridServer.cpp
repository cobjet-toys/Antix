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
	updateDrawerFlag = 0;
}

int GridServer::initGridGame()
{
    /*
	gridGameInstance = new GridGame(m_uId, m_teamsAvailable, m_robotsPerTeam, m_idRangeFrom, m_idRangeTo); // needs to not do this in grid game constructor!
    */

	printf("id=%lu, teams=%lu, robots=%lu, idfrom=%lu, idto=%lu\n", (unsigned long)m_uId, (unsigned long)m_teamsAvailable,(unsigned long) m_robotsPerTeam, (unsigned long)m_idRangeFrom, (unsigned long)m_idRangeTo);
 
    // parameters: gridid, num_of_teams, robots_per_team, id_from, id_to
    gridGameInstance = new GridGame(1, 2, 25000, 10, 50009);
    GridGame* gridGameInstance2 = new GridGame(2, 2, 5, 20, 29);

    Msg_RobotInfo newrobot;
    newrobot.robotid = 400;
    newrobot.x_pos = 2.5;
    newrobot.y_pos = 2.5;

    std::vector<int> robots;
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
    gridGameInstance->returnSensorData(robots, sensed_items);

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

    DEBUGPRINT("=====Get Sensor Data\n");
    gridGameInstance->returnSensorData(robots, sensed_items);
	gridGameInstance->printPopulation();

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
	    //gridGameInstance->printPopulation();
        DEBUGPRINT("=====Printing population for grid 2\n");
	    gridGameInstance2->printPopulation();
        i++;
    }
	return 0;
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

    for(uint32_t frame = 0; true; frame++)
    {
        usleep(FRAME_FREQUENCY);

        try
        {
            printf("UpdateDrawer return[%d]: %d\n", frame, grid->updateDrawer(frame));
        }
        catch(std::exception & e)
        {
            printf("Failed To Updated Drawer!\n");
            perror(e.what());
            return NULL;
        }
    }

    printf("Finished Execution!!\n");   
	return NULL;
}


int GridServer::handler(int fd)
{
	TcpConnection * l_curConnection = m_Clients[fd];
	
	if (l_curConnection == NULL)
    {
		return -1; // no such socket descriptor
	}
	
	uint16_t l_sender=-1, l_senderMsg =-1;
	
	NetworkCommon::recvHeader(l_sender, l_senderMsg, l_curConnection);

	DEBUGPRINT("sender=%u, message=%u\n", l_sender, l_senderMsg);
	
	if (l_sender == -1 || l_senderMsg == -1) return -1; // bad messages
	
	switch (l_sender)
	{
        case SENDER_GRIDSERVER:
        {
            switch(l_senderMsg)
            {
                case(MSG_BOUNDARYUPDATE):
                {
                    Msg_header l_Header;
                    Msg_MsgSize l_Size;
              
                    unsigned char l_SizeBuffer[l_Size.size];

                    if (l_curConnection->recv(l_SizeBuffer, l_Size.size) == -1)
				    {
							DEBUGPRINT("Couldn't receive the number of boundary robo updates.\n");
							return -1;
					}
                    unpack(l_SizeBuffer, Msg_MsgSize_format, &l_Size.msgSize);

                    DEBUGPRINT("Received grid boundary robo information for %hd robos\n", l_Size.msgSize);

                    Msg_RobotInfo l_RoboInfo;
                    std::vector<Msg_RobotInfo> l_RoboUpdates;
                
                    unsigned char l_Buffer[l_RoboInfo.size*l_Size.msgSize];
                    int l_Offset = 0;

                    for (int i = 0; i < l_Size.msgSize; i++)
                    {
                        unpack(l_Buffer+l_Offset, Msg_RobotInfo_format, &l_RoboInfo.robotid, &l_RoboInfo.x_pos, &l_RoboInfo.y_pos, &l_RoboInfo.speed, &l_RoboInfo.angle, &l_RoboInfo.puckid, &l_RoboInfo.gridid);
                        l_RoboUpdates.push_back(l_RoboInfo);
                        l_Offset += l_RoboInfo.size;
                    }

                    gridGameInstance->updateRobots(l_RoboUpdates);

                }
                break;
            }
        }
        break;
        case SENDER_CONTROLLER:
        {
            switch(l_senderMsg)
            {
                case(MSG_GRIDNEIGHBOURS):
                {
                    Msg_MsgSize l_NumNeighbours;                    
                    unsigned char l_SizeBuffer[l_NumNeighbours.size];

                    //Receive and unpack the number of neighbours.
                    if (l_curConnection->recv(l_SizeBuffer, l_NumNeighbours.size) == -1)
				    {
							DEBUGPRINT("Couldn't receive the number of neighbour.\n");
							return -1;
					}
                    unpack(l_SizeBuffer, Msg_MsgSize_format, &l_NumNeighbours.msgSize);

                    DEBUGPRINT("Received grid neighbour information for %hd neighbours\n", l_NumNeighbours.msgSize);

                    Msg_GridNeighbour l_Neighbour;
                    //Create a buffer big enough to hold all of the expected neighbours.
                    unsigned int l_SizeOfNeighbourInfo = l_Neighbour.size*l_NumNeighbours.msgSize;
                    unsigned char l_NeighbourBuffer[l_SizeOfNeighbourInfo];

                    if (l_curConnection->recv(l_NeighbourBuffer, l_SizeOfNeighbourInfo) == -1)
				    {
							DEBUGPRINT("Couldn't receive the grid neighbours.\n");
							return -1;
					}                    unsigned int l_Offset = 0;

                    for (int i = 0; i < l_NumNeighbours.msgSize;i++)
                    {
                        unpack(l_NeighbourBuffer+l_Offset, Msg_gridNeighbour_format, &l_Neighbour.position,
                                &l_Neighbour.ip, &l_Neighbour.port);
                        l_Offset += l_Neighbour.size;
                        printf("Received a new neighbour at position %hd, with IP %s and Port %s\n",
                                l_Neighbour.position, l_Neighbour.ip, l_Neighbour.port);
                        m_GridPosToFd[l_Neighbour.position] = initConnection(l_Neighbour.ip, l_Neighbour.port); 
                        //TODO Handle new neighbour.
                    }
				}
                break;
				case (MSG_REQUESTGRIDWAITING):
				{
					m_ControllerFd = fd;
					DEBUGPRINT("STATUS: Controller asked if this grid is ready\n");
					Msg_GridId l_gridId;
					unsigned char l_gridIdBuff[l_gridId.size];
					Msg_header l_header;
					
					
					if (l_curConnection->recv(l_gridIdBuff, l_gridId.size) < 0)
					{
						DEBUGPRINT("GRID_SERVER FAILED:\t failed to receive id from controller.\n");
						return -1;
					}
					
					unpack(l_gridIdBuff, Msg_GridId_format, &m_uId);
					
					Msg_GridRequestIdRage l_reqGridRange;
					
					unsigned char l_gridBuff[l_header.size + l_gridId.size + l_reqGridRange.size];
					
					if (NetworkCommon::packHeader(l_gridBuff, SENDER_GRIDSERVER, MSG_RESPONDGRIDWAITING) < 0)
					{
						DEBUGPRINT("GRID_SERVER FAILED:\t Cannot pack ready header to controller.\n");
						return -1;
					}
					
					if (pack(l_gridBuff+l_header.size, "lll", m_uId, (unsigned long)m_teamsAvailable, (unsigned long)m_robotsPerTeam) != (l_gridId.size + l_reqGridRange.size))
					{
						DEBUGPRINT("GRID_SERVER FAILED:\t Cannot send grid waiting header to controller");
						return -1;
					}
					uint32_t id;

					if (NetworkCommon::sendMsg(l_gridBuff, l_header.size+l_gridId.size+l_reqGridRange.size, l_curConnection) < 0)
					{
						DEBUGPRINT("GRID_SERVER FAILED:\t Cannot send grid waiting header to controller");
						return -1;
					}
					
					DEBUGPRINT("GRID_SERVER STATUS:\t Send grid waiting header to controller");
					
				}
				break;
				case (MSG_RESPONDGRIDRANGE):
				{
					DEBUGPRINT("GRID_SERVER STATUS:\t Receiving range information\n");
					
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
					
				}
				break;
            }
        }
        break;
        case SENDER_CLIENT:
			
            switch(l_senderMsg)
            {
				case (MSG_REQUESTSENSORDATA):
				{
					
					Msg_MsgSize l_msgSize;
					NetworkCommon::recvMessageSize(l_msgSize, l_curConnection);
					int l_numRobots = l_msgSize.msgSize;
					
					if (l_numRobots <= 0)
					{
						DEBUGPRINT("Requested sensor data for <=0 Robots. Game error.\n");
						return -1;
					}
					
					DEBUGPRINT("Client has requested actions for %i robots\n", l_numRobots);
					
					std::vector<int> l_robotIdVector;
					l_robotIdVector.clear();
					
					Msg_RobotId l_robotId;
					unsigned char l_robotIdBuff[l_robotId.size * l_numRobots];
					
					for (int i = 0; i < l_numRobots; i++)
					{
						if (l_curConnection->recv(l_robotIdBuff+(l_robotId.size * i), l_robotId.size) == -1)
						{
							DEBUGPRINT("Could not get robot id for returnSensorData\n");
							return 0;
						}
						unsigned int id;
						unpack(l_robotIdBuff+(l_robotId.size * i),Msg_RobotId_format, &id);
						l_robotIdVector.push_back(id);
						DEBUGPRINT("%i\n", id);
					}

					int l_robotsTotal = l_robotIdVector.size(); // how many robots have sensory data
					int l_totalSensed = 0; // total of all the robots sensory data *how many sensed items in total*
					
					std::map<int, std::vector<sensed_item> > sensed_items_map; // has map that contains a list for each robot in request list
					/*
					 ****************************************************************************************
					 * gridGameInstance.returnSensorData(robot_ids, &sensed_items_map, l_totalSensed); // collect the sensory information
					 ****************************************************************************************
					*/
					
					sensed_item s1 ;
					
					l_totalSensed = 5;
					std::vector<sensed_item> a1;
					
					for (int i =0; i< l_totalSensed; i++)
					{
						s1.id = i;
						s1.x = i;
						s1.y = i;
						a1.push_back(s1);
					}
					DEBUGPRINT("got all sensory data\n");
					for (int i = 0; i < l_robotsTotal; i++)
					{
						sensed_items_map[i] = a1;
					}
					DEBUGPRINT("got all robot <= sensory data total of %zui robots with %zui sensory objects\n", a1.size(),sensed_items_map.size());
					
					Msg_header l_header = {SENDER_GRIDSERVER, MSG_RESPONDSENSORDATA}; // header for response
					memset(&l_msgSize, 0 , l_msgSize.size);
					
					l_msgSize.msgSize = l_robotsTotal;
					
					Msg_SensedObjectGroupHeader l_robotHeader; // for each robot
					Msg_SensedObjectGroupItem l_sensedObject; // for each sensed item
					
					unsigned int l_responseMsgSize = 0;
					
					l_responseMsgSize += l_header.size; // append header size
					l_responseMsgSize += l_msgSize.size; // append size of msg length 
					
					l_responseMsgSize += (l_robotsTotal * l_robotHeader.size); // append the total size for number of robot ids
					l_responseMsgSize += (l_totalSensed * l_robotsTotal * l_sensedObject.size); // all sensed items
					
					DEBUGPRINT("msg size: %i\n", l_responseMsgSize);
					
					unsigned char msgBuffer[l_responseMsgSize];
					
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
					
					DEBUGPRINT("AFTER PACKING SIZE %d\n", l_position);
					
					std::map<int, std::vector<sensed_item> >::iterator map_end = sensed_items_map.end(); 
					 // sensed item iterator
					
					DEBUGPRINT("Set up iterator\n");
					
									
					for (std::map<int, std::vector<sensed_item> >::iterator it = sensed_items_map.begin(); it != map_end; it++)
					{
						// for each robot requested
						l_robotHeader.id = it->first;
						l_robotHeader.objectCount = it->second.size();
						DEBUGPRINT("PACKING robot %d with %d sensory objects\n", l_robotHeader.id, l_robotHeader.objectCount);
						if (pack(msgBuffer+l_position, Msg_SensedObjectGroupHeader_format, l_robotHeader.id, l_robotHeader.objectCount) != l_robotHeader.size)
						{
							DEBUGPRINT("Could not pack robot header\n");
							return -1;
						}
						
						l_position += l_robotHeader.size;
						DEBUGPRINT("CURRENT POSITION SIZE %d\n", l_position);
						
						std::vector<sensed_item>::iterator sensedItemIt = it->second.end(); 
						
						for (std::vector<sensed_item>::iterator vecIt = it->second.begin(); vecIt != sensedItemIt; vecIt++)
						{
							// for each object seen by such robot
							l_sensedObject.y = vecIt->y;
							l_sensedObject.x = vecIt->x;
							l_sensedObject.id = vecIt->id;
							DEBUGPRINT("Packing sensed object id=%d x=%d y=%d | of buffer %d\n", l_sensedObject.id, l_sensedObject.x, l_sensedObject.y, l_position);
							if (pack(msgBuffer+l_position, Msg_SensedObjectGroupItem_format, l_sensedObject.id, l_sensedObject.x, l_sensedObject.y) != l_sensedObject.size)
							{
								DEBUGPRINT("Could not pack robot header\n");
								return -1;
							}
							
							l_position += l_sensedObject.size;
						}
						
					}
					
					if (l_curConnection->send(msgBuffer, l_responseMsgSize) == -1)
					{
						DEBUGPRINT("failed to send");
						return -1;
					}
					
					DEBUGPRINT("Sent Response\n");
						
					return 0;
					
				} // end of message sensor datel_Results[i].robotid, l_Results[i].x_pos, l_Results[i].y_pos, l_Results[i].speed, l_Results[i].angle, l_Results[i].puckid
                break;
				
				case (MSG_PROCESSACTION):
				{
                    Msg_header l_Header;
                    Msg_MsgSize l_Size;
                    Msg_Action l_Action;

                    NetworkCommon::recvMessageSize(l_Size, l_curConnection);

                    unsigned int l_MessageSize = l_Size.msgSize*l_Action.size;
                    unsigned char l_ActionBuffer[l_MessageSize];
                  	if (l_curConnection->recv(l_ActionBuffer, l_MessageSize) == -1)
					{
						DEBUGPRINT("Could not get Msg_Actions for processs actions\n");
						return 0;
					}

                    std::vector<Msg_Action> l_Actions;
                    unsigned int l_Offset = 0;
                    DEBUGPRINT("Received a total of %d action requests\n", l_Size.msgSize);
                    for (int i = 0; i < l_Size.msgSize; i++)
                    {
                        unpack(l_ActionBuffer+l_Offset, Msg_Action_format, &l_Action.robotid, &l_Action.action, 
                                &l_Action.speed, &l_Action.angle);
                        l_Actions.push_back(l_Action);
                        l_Offset += l_Action.size;
                    }

                    std::vector<Msg_RobotInfo> l_Results;

                    
                    std::vector<std::pair<int, std::vector<Msg_RobotInfo> > > robots_to_pass;
                    gridGameInstance->processAction(l_Actions, &l_Results, &robots_to_pass);

                    DEBUGPRINT("Received %zu results\n", l_Results.size());

                    Msg_RobotInfo l_Result;
                    l_MessageSize = l_Header.size+l_Size.size+(l_Result.size*l_Size.msgSize);

                    l_Offset = 0;
                    unsigned char l_ResultsBuffer[l_MessageSize];
                    NetworkCommon::packHeader(l_ResultsBuffer+l_Offset, SENDER_GRIDSERVER,MSG_RESPONDPROCESSACTION);
                    l_Offset += l_Header.size;

                    pack(l_ResultsBuffer+l_Offset, Msg_MsgSize_format, l_Size.msgSize);
                    l_Offset += l_Size.size;

                    for (int i = 0; i < l_Size.msgSize; i++)
                    {
                        pack(l_ResultsBuffer+l_Offset, Msg_RobotInfo_format, l_Results[i].robotid, l_Results[i].x_pos, l_Results[i].y_pos, l_Results[i].speed, l_Results[i].angle, l_Results[i].puckid, l_Results[i].gridid);
                        l_Offset += l_Result.size;
                    }

                    NetworkCommon::sendMsg(l_ResultsBuffer, l_MessageSize, l_curConnection);

                    l_Header.sender = SENDER_GRIDSERVER;
                    l_Header.message = MSG_BOUNDARYUPDATE;

                    Msg_RobotInfo l_RoboInfo;
                    int l_BoundarySize = robots_to_pass.size();
                    for(int i = 0; i < l_BoundarySize; i++)
                    {
                        l_Offset = 0;

                        TcpConnection* l_GridCon = m_Clients[m_GridPosToFd[robots_to_pass[i].first]];
                        
                        l_Size.msgSize = robots_to_pass[i].second.size();
                        l_MessageSize = l_Header.size+l_Size.size+(l_Size.msgSize * l_RoboInfo.size);

                        unsigned char l_BoundaryBuffer[l_MessageSize];
                        
                        NetworkCommon::packHeader(l_BoundaryBuffer+l_Offset, SENDER_GRIDSERVER, MSG_BOUNDARYUPDATE); 
                        l_Offset += l_Header.size;

                        pack(l_BoundaryBuffer+l_Offset, Msg_MsgSize_format, l_Size.msgSize);
                        l_Offset += l_Size.size;

                        for (int a = 0; a < l_Size.msgSize; a++)
                        {
                            Msg_RobotInfo& l_RoboToPack = robots_to_pass[i].second[a];
                            pack(l_BoundaryBuffer+l_Offset, Msg_RobotInfo_format, l_RoboToPack.robotid, l_RoboToPack.x_pos, l_RoboToPack.y_pos, l_RoboToPack.speed, l_RoboToPack.angle, l_RoboToPack.puckid, l_RoboToPack.gridid);
                            l_Offset += l_RoboToPack.size;
                        }
						DEBUGPRINT("Prepairing to send\n");
                        NetworkCommon::sendMsg(l_BoundaryBuffer, l_MessageSize, l_GridCon);
                        DEBUGPRINT("Sent Message\n");
                    }

					/*int l_numRobots = -1;
					
					NetworkCommon::requestMessageSize(l_numRobots, l_curConnection);
					
					if (l_numRobots <= 0)
					{
						DEBUGPRINT("Requested sensor data for <=0 Robots. Game error.\n");
						return -1;
					}
					
					DEBUGPRINT("Client has requested actions for %i robots\n", l_numRobots);
					
					Msg_header l_header = {SENDER_GRIDSERVER, MSG_RESPONDSENSORDATA}; // header for response
					Msg_MsgSize l_msgSize;
					
					int l_position = l_header.size + l_msgSize.size;
					
					Msg_RobotId l_robotId;
					Msg_Action l_action;
					
					int l_msgSize = (l_robotId.size * l_numRobots) + (l_numRobots * l_action.size);
					
					unsigned char l_robotIdBuff[l_msgSize];
					
					for (int i = 0; i < l_numRobots; i++)
					{
						l_position = i*(l_robotId.size + l_action.size);
						
						if (l_curConnection->recv(l_robotIdBuff+l_position, l_robotId.size) == -1)
						{
							DEBUGPRINT("Could not get robot id for returnSensorData\n");
							return 0;
						}
						unsigned int id;
						unpack(l_robotIdBuff+l_position,Msg_RobotId_format, &id);

						if (l_curConnection->recv(l_robotIdBuff+l_position+l_robotId.size, l_action.size) == -1)
						{
							DEBUGPRINT("Could not get robot id for returnSensorData\n");
							return 0;
						}
						memset(&l_action, 0, l_action.size);
						unpack(l_robotIdBuff+l_position+l_robotId.size, Msg_Action_format, &l_action.action, &l_action.speed, &l_action.angle);
						
						
					}*/
					
					return 0;
					
				}// end of message process action
                break;
				
				case (MSG_REQUESTINITTEAM):
				{
					DEBUGPRINT("MSG_REQUESTINITTEAM\n");
					Msg_TeamInit l_Team;
					std::vector<Msg_InitRobot> * l_robots = new std::vector<Msg_InitRobot>;
					gridGameInstance->getRobots(l_Team, l_robots);

					DEBUGPRINT("%zu\n", l_robots->size());
					Msg_header l_header;
					Msg_MsgSize l_size;
					Msg_InitRobot l_robot;
					size_t l_length = l_header.size + l_Team.size + l_size.size + (l_robots->size() * l_robot.size);
					unsigned char * l_message = new unsigned char[l_length];
					DEBUGPRINT("a1\n");
					int l_offset = 0;
					NetworkCommon::packHeader(l_message, SENDER_GRIDSERVER, MSG_RESPONDINITTEAM);
					


					l_offset += l_header.size;
					DEBUGPRINT("a2\n");

					pack(l_message+l_offset, Msg_TeamInit_format, l_Team.id, l_Team.x, l_Team.y);

					l_offset += l_Team.size;

					l_size.msgSize = l_robots->size();
					
					if (pack(l_message+l_offset, Msg_MsgSize_format, l_size.msgSize) != l_size.size)
					{
						return -1;
					}
					l_offset += l_size.size;
					DEBUGPRINT("a3\n");
					for( int i = 0; i < m_robotsPerTeam;i ++ )
					{
						l_robot.id = l_robots->at(i).id;
						l_robot.x = l_robots->at(i).x;
						l_robot.y = l_robots->at(i).y;
						DEBUGPRINT("ID: %d, X: %f, F: %f \n", l_robot.id, l_robot.x, l_robot.y);
						
						if (pack(l_message+l_offset, Msg_InitRobot_format, l_robot.id, l_robot.x, l_robot.y) != l_robot.size)
						{
							return -1;
						}
						l_offset += l_robot.size;
					}
					
					if (NetworkCommon::sendMsg(l_message, l_length, l_curConnection) < 0)
					{
						return -1;
					}
					
					return 0;
				}
                break;
				
				case (MSG_CONFIRMTEAM):
				{
					DEBUGPRINT("Grid Responded\n");
					m_teamsConfirmed++;
					if (m_teamsConfirmed == m_teamsAvailable)
					{
						TcpConnection * contCon = m_Clients[m_ControllerFd];
						
						Msg_header l_header;
						Msg_GridId l_msg;
						
						unsigned char message[l_header.size + l_msg.size];
						if (NetworkCommon::packHeader(message, SENDER_GRIDSERVER,MSG_GRIDCONFIRMSTARTED) < 0)
						{
							DEBUGPRINT("GRID_SERVER FAILED:\t Failed to pack header confirm started\n");
							return -1;
						}
						if (pack(message+l_header.size, Msg_GridId_format, m_uId) != l_msg.size)
						{
							DEBUGPRINT("GRID_SERVER FAILED:\t Failed to pack id for confirm started\n");
							return -1;
						}
						
						if (NetworkCommon::sendMsg(message, l_header.size+l_msg.size, contCon) < 0)
						{
							DEBUGPRINT("GRID_SERVER FAILED:\t Failed to send confirm started message\n");
							return -1;
						}
						DEBUGPRINT("GRID_SERVER STATUS:\t sent grid server confirmation\n");
					}				
					
					return 0;
				}
				
				default:
				{
					DEBUGPRINT("No matching message handle for client case.\n");
					return -1;
				}
			} // end of sender
		case SENDER_DRAWER:
            switch(l_senderMsg)
            {
                case(MSG_SETDRAWERCONFIG):
                {
                    //DEBUGPRINT("Recieved Drawer Instruction\n");

                    Msg_DrawerConfig configData;
                    unsigned char configDataBuf[configData.size];

                    if (l_curConnection->recv(configDataBuf, configData.size) == -1)
					{
						DEBUGPRINT("Could not receive a config data.");
						return -1;
					}
					unpack(configDataBuf, Msg_DrawerConfig_format, &configData.send_data, &configData.data_type, &configData.tl_x, &configData.tl_y, &configData.br_x, &configData.br_y);

                    //DEBUGPRINT("Config: send_data=%c, data_type=%c, tl_x=%f, tl_y=%f, br_x=%f, br_y=%f\n",
                           //configData.send_data, configData.data_type, configData.tl_x, configData.tl_y, configData.br_x, configData.br_y);

					// If this is the first message from the Drawer, send team home data and create update thread
					if (!m_drawerConn)
					{
						//Send team data
						Msg_header l_header = {SENDER_GRIDSERVER, MSG_GRIDTEAMS}; // header for response
						Msg_MsgSize l_msgSize;	
						l_msgSize.msgSize = 1; //number of teams with homes on grid

						Msg_TeamInit l_TeamInfo;	//for each object

						unsigned int l_responseMsgSize = 0;	
						l_responseMsgSize += l_header.size; 					// append header size	
						l_responseMsgSize += l_msgSize.size;					// append msgSize size
						l_responseMsgSize += (l_msgSize.msgSize * l_TeamInfo.size); // append the total size for number of team info

						unsigned char msgBuffer[l_responseMsgSize];

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

						for(int i = 0; i < l_msgSize.msgSize; i++)
						{
							// for each team being pushed
							l_TeamInfo.id = i;
							l_TeamInfo.x = float((rand()%600));		//fake data
							l_TeamInfo.y = float((rand()%600));		//fake data
										   
							if (pack(msgBuffer+l_position, Msg_TeamInit_format,
									 l_TeamInfo.id, l_TeamInfo.x, l_TeamInfo.y ) != l_TeamInfo.size)
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
    Msg_header l_Header = {SENDER_GRIDSERVER, MSG_GRIDDATAFULL };


    Msg_MsgSize l_Size;
    Msg_RobotInfo l_RoboInfo;

    std::vector<Msg_RobotInfo> objects;
    gridGameInstance->getPopulation(&objects);

    l_Size.msgSize = objects.size();

    unsigned int l_Offset = 0;
    unsigned int l_MessageSize = l_Header.size+l_Size.size+(l_RoboInfo.size*l_Size.msgSize);
    unsigned char l_Buffer[l_MessageSize];

    NetworkCommon::packHeader(l_Buffer+l_Offset, SENDER_GRIDSERVER, MSG_GRIDDATAFULL);
    l_Offset += l_Header.size;

    pack(l_Buffer+l_Offset, Msg_MsgSize_format, l_Size.msgSize);
    l_Offset += l_Size.size;

    for (int i = 0; i < l_Size.msgSize; i++)
    {
        pack(l_Buffer+l_Offset, Msg_RobotInfo_format, objects[i].robotid, objects[i].x_pos, objects[i].y_pos, 0, 0, 0 ); 
        unpack(l_Buffer+l_Offset, Msg_RobotInfo_format,
                                &l_RoboInfo.robotid, &l_RoboInfo.x_pos, &l_RoboInfo.y_pos, &l_RoboInfo.angle, &l_RoboInfo.puckid, &l_RoboInfo.gridid );

        DEBUGPRINT("Object: id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%d\tgrid=%d\n",
                        	        l_RoboInfo.robotid, l_RoboInfo.x_pos, l_RoboInfo.y_pos, l_RoboInfo.angle, l_RoboInfo.puckid, l_RoboInfo.gridid );
                         
       // DEBUGPRINT("id: %d xpos: %f ypos: %f\n", objects[i].robotid, objects[i].x_pos, objects[i].y_pos);
        l_Offset += l_RoboInfo.size;
    }

    if(!m_drawerConn || m_drawerConn->send(l_Buffer, l_MessageSize) == -1)
    {
        DEBUGPRINT("Failed to send\n");
        return -1;
    }
 
/*    int m_totalRobots = 10;		
    int m_totalPucks = 5;
    int l_totalObjects = m_totalRobots + m_totalPucks;


    // Creates an array of random values, to draw the pucks -- TEMPORARY TESTING -- //
    srand(time(NULL));
    float randPuckVals[m_totalPucks * 2];
    for(int i = 0; i < m_totalPucks * 2; i++){randPuckVals[i] = (float)(rand()%600);}*/
    // Creates an array of random values for robot positions -- TEMPORARY TESTING -- //
    //float robotOrientation[this->m_totalRobots];
    //for(int i = 0; i < this->m_totalRobots; i++){robotPosition[i] = (float)(rand()%100)/100;}

    // Filled out fake data
    /*
    float robotPosition[m_totalRobots][2];
    for(int i = 0; i < m_totalRobots; i++){
        for(int j = 0; j < 2; j++){
            robotPosition[i][j] = (float)(rand()%600);
        }
    }
    */

/*
    Msg_header l_header = {SENDER_GRIDSERVER, MSG_GRIDDATAFULL}; // header for response
    Msg_MsgSize l_msgSize;
    memset(&l_msgSize, 0, l_msgSize.size);	
    l_msgSize.msgSize = l_totalObjects; 	//robots + pucks
    DEBUGPRINT("Sending %d robots, %d pucks.\n", m_totalRobots, m_totalPucks );

    Msg_RobotInfo l_ObjInfo;	//for each object

    unsigned int l_responseMsgSize = 0;	
    l_responseMsgSize += l_header.size; 					// append header size	
    l_responseMsgSize += l_msgSize.size;					// append msgSize size
    l_responseMsgSize += (l_totalObjects * l_ObjInfo.size); // append the total size for number of object info

    unsigned char msgBuffer[l_responseMsgSize];

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
    //printf("Sending %d objects.\n", l_msgSize.msgSize);

    l_position += l_msgSize.size; // shift by robot size header

    // Getting GameObjects from population from gridGameInstance
    std::vector<Msg_RobotInfo> objects;
    gridGameInstance->getPopulation(&objects);

    std::vector<Msg_RobotInfo>::iterator endit = objects.end();
    //for(int i = 0; i < m_totalRobots; i++)
    for(std::vector<Msg_RobotInfo>::iterator it = objects.begin(); it != endit; it++)
    {
        // Computes robots altered position (Random)
        //robotPosition[i][0] += float((rand()%200)-100)/50;
        //robotPosition[i][1] += float((rand()%200)-100)/50;

        //float posX = robotPosition[i][0];
        //float posY = robotPosition[i][1];
        //float orientation = 1.0;

        // for each object being pushed

        //DEBUGPRINT("Expected: newInfo.id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%c\n",
                   //l_ObjInfo.id, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.has_puck );
                       
        if (pack(msgBuffer+l_position, Msg_RobotInfo_format,
                 (*it).robotid, (*it).x_pos, (*it).y_pos, 0, 0 ) != l_ObjInfo.size)
        {
            DEBUGPRINT("Could not pack robot header\n");
            return -1;
        }

        l_position += l_ObjInfo.size;
        
        //DEBUGPRINT("CURRENT POSITION SIZE %d\n", l_position);		
    }

    
    for(int i = 0; i < m_totalPucks; i++)
    {
        float posX = randPuckVals[(2*i)];
        float posY = randPuckVals[(2*i) + 1];

        // for each object being pushed
        l_ObjInfo.robotid = Antix::writeId(i, PUCK);
        l_ObjInfo.x_pos = posX;
        l_ObjInfo.y_pos = posY;
        l_ObjInfo.angle = 1.0;
        l_ObjInfo.puckid = '0';

        //DEBUGPRINT("Object: newInfo.id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%c\n",
        //           l_ObjInfo.id, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.has_puck);
                         
        if(pack(msgBuffer+l_position, Msg_RobotInfo_format,
           &l_ObjInfo.robotid, &l_ObjInfo.x_pos, &l_ObjInfo.y_pos, &l_ObjInfo.angle, &l_ObjInfo.puckid) != l_ObjInfo.size)
        {
            DEBUGPRINT("Could not pack robot header\n");
            return -1;
        }

        l_position += l_ObjInfo.size;
        //DEBUGPRINT("CURRENT POSITION SIZE %d\n", l_position);		
    }
    */

   
    
    return 0;
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

