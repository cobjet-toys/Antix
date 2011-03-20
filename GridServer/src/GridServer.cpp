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

    robot_info newrobot;
    newrobot.id = 400;
    newrobot.x_pos = 5;
    newrobot.y_pos = 5;

    std::vector<int> teams;
    teams.push_back(2);

    std::vector<int> robots;
    robots.push_back(2);

    std::map<int, std::vector<sensed_item> >* sensed_items_map;

    DEBUGPRINT("=====Create Game\n");

    // parameters: gridid, num_of_teams, robots_per_team, id_from, id_to
    gridGameInstance = new GridGame(1, 2, 10, 1, 10);
    //DEBUGPRINT("=====Initialize teams\n");
    //std::vector<robot_info>* robot_info_vector;
    //gridGameInstance->initializeTeam(teams, robot_info_vector);
    gridGameInstance->printPopulation();
    DEBUGPRINT("=====Unregister Robot\n");
    gridGameInstance->unregisterRobot(1);
    gridGameInstance->printPopulation();
    DEBUGPRINT("=====Register Robot\n");
    gridGameInstance->registerRobot(newrobot);
    gridGameInstance->printPopulation();
    DEBUGPRINT("=====Get Sensor Data\n");
    gridGameInstance->returnSensorData(teams, sensed_items_map);

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
	
	NetworkCommon::requestHeader(l_sender, l_senderMsg, l_curConnection);

	DEBUGPRINT("%u, %u\n", l_sender, l_senderMsg);
	
	if (l_sender == -1 || l_senderMsg == -1) return -1; // bad messages
	
	switch (l_sender)
	{
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
                        //TODO Handle new neighbour.
                    }
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
					NetworkCommon::requestMessageSize(l_msgSize, l_curConnection);
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
					DEBUGPRINT("got all robot <= sensory data total of %ui robots with %ui sensory objects\n", a1.size(),sensed_items_map.size());
					
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
					
					printf("msg size: %i\n", l_responseMsgSize);
					
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
							l_sensedObject.robotid = vecIt->id;
							DEBUGPRINT("Packing sensed object id=%d x=%d y=%d | of buffer %d\n", l_sensedObject.robotid, l_sensedObject.x, l_sensedObject.y, l_position);
							if (pack(msgBuffer+l_position, Msg_SensedObjectGroupItem_format, l_sensedObject.robotid, l_sensedObject.x, l_sensedObject.y) != l_sensedObject.size)
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
					
				} // end of message sensor date
                break;
				
				case (MSG_PROCESSACTION):
				{
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
                    unsigned int ROBOSPERTEAM = 1000;                    
					Msg_header l_Header;
					Msg_MsgSize l_Size = {ROBOSPERTEAM};//INITIALIZE WITH FUNCTION THAT RETRIEVES NUMBER OF ROBS / TEAM
					Msg_InitRobot l_RoboInfo = {3, 0.1, 0.1};

                    unsigned int l_MessageSize = l_Header.size + l_Size.size + (ROBOSPERTEAM * l_RoboInfo.size); 
                    unsigned char l_Buffer[l_MessageSize];

                    l_Header.sender = SENDER_GRIDSERVER;
                    l_Header.message = MSG_RESPONDINITTEAM;

                    unsigned int l_Offset = 0;
                    pack(l_Buffer+l_Offset, Msg_header_format, l_Header.sender, l_Header.message);
                    l_Offset += l_Header.size;
                    
                    pack(l_Buffer+l_Offset, Msg_MsgSize_format, l_Size.msgSize);
                    l_Offset += l_Size.size;

                    for(int i = 0; i < ROBOSPERTEAM; i++)
                    {
                        pack(l_Buffer+l_Offset, Msg_InitRobot_format, l_RoboInfo.id, l_RoboInfo.x, l_RoboInfo.y);
                        l_Offset += l_RoboInfo.size;
                    }

                    if (l_curConnection->send(l_Buffer, l_MessageSize) == -1)
					{
						DEBUGPRINT("failed to send");
						return -1;
					}

					return 0;
				}
                break;
				
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
                    // Sets the drawer TCP Connection reference, if it is NULL                    
                    if(!m_drawerConn)
                    {
                        m_drawerConn = l_curConnection;
                    }

                    printf("Recieved Drawer Instruction\n");

                    Msg_DrawerConfig configData;
                    unsigned char configDataBuf[configData.size];

                    if (l_curConnection->recv(configDataBuf, configData.size) == -1)
					{
						DEBUGPRINT("Could not receive a config data.");
						return -1;
					}
					unpack(configDataBuf, Msg_DrawerConfig_format, &configData.send_data, &configData.data_type, &configData.left_x, &configData.left_y, &configData.right_x, &configData.right_y);

                    printf("Config: send_data=%c, data_type=%c, left_x=%f, bottom_y=%f, right_x=%f, top_y=%f\n",
                           configData.send_data, configData.data_type, configData.left_x, configData.left_y, configData.right_x, configData.right_y);

                    // TODO - Make it only initialize a single pThread
                    pthread_t thread1;
                    int iret1 = pthread_create(&thread1, NULL, drawer_function, (void *)this);
                    if(iret1 != 0)
                    {
                        perror("pthread failed");
                        return -1;
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
    printf("----updateDrawer---------\n");

    int m_totalRobots = 10;
    int m_totalPucks = 0;
    int l_totalObjects = m_totalRobots + m_totalPucks;


    // Creates an array of random values, to draw the pucks -- TEMPORARY TESTING -- //
    srand(time(NULL));
    float randPuckVals[m_totalPucks * 2];
    for(int i = 0; i < m_totalPucks * 2; i++){randPuckVals[i] = (float)(rand()%600);}
    // Creates an array of random values for robot positions -- TEMPORARY TESTING -- //
    //float robotOrientation[this->m_totalRobots];
    //for(int i = 0; i < this->m_totalRobots; i++){robotPosition[i] = (float)(rand()%100)/100;}
    float robotPosition[m_totalRobots][2];
    for(int i = 0; i < m_totalRobots; i++){for(int j = 0; j < 2; j++){robotPosition[i][j] = (float)(rand()%600);}}


    Msg_header l_header = {SENDER_GRIDSERVER, MSG_GRIDDATAFULL}; // header for response
    Msg_MsgSize l_msgSize;
    memset(&l_msgSize, 0, l_msgSize.size);	
    l_msgSize.msgSize = l_totalObjects; 	//robots + pucks

    Msg_RobotInfo l_ObjInfo;	//for each object

    unsigned int l_responseMsgSize = 0;	
    l_responseMsgSize += l_header.size; 					// append header size	
    l_responseMsgSize += l_msgSize.size;					// append msgSize size
    l_responseMsgSize += (l_totalObjects * l_ObjInfo.size); // append the total size for number of object info
    printf("msg size: %i\n", l_responseMsgSize);

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

    for(int i = 0; i < m_totalRobots; i++)
    {
        // Computes robots altered position (Random)
        robotPosition[i][0] += float((rand()%200)-100)/50;
        robotPosition[i][1] += float((rand()%200)-100)/50;

        float posX = robotPosition[i][0];
        float posY = robotPosition[i][1];
        float orientation = 1.0;

        // for each object being pushed
        l_ObjInfo.id = i + 1024;
        l_ObjInfo.x_pos = posX;
        l_ObjInfo.y_pos = posY;
        l_ObjInfo.angle = orientation;
        l_ObjInfo.has_puck = i%2 == 0 ? 'T' : 'F';

        //DEBUGPRINT("Expected: newInfo.id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%c\n",
                   //l_ObjInfo.id, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.has_puck );
                       
        if (pack(msgBuffer+l_position, Msg_RobotInfo_format,
                 l_ObjInfo.id, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.has_puck ) != l_ObjInfo.size)
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
        l_ObjInfo.id = i;
        l_ObjInfo.x_pos = posX;
        l_ObjInfo.y_pos = posY;
        l_ObjInfo.angle = 1.0;
        l_ObjInfo.has_puck = 'F';

        //DEBUGPRINT("Object: newInfo.id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%c\n",
        //           l_ObjInfo.id, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.has_puck);
                         
        if(pack(msgBuffer+l_position, Msg_RobotInfo_format,
           &l_ObjInfo.id, &l_ObjInfo.x_pos, &l_ObjInfo.y_pos, &l_ObjInfo.angle, &l_ObjInfo.has_puck) != l_ObjInfo.size)
        {
            DEBUGPRINT("Could not pack robot header\n");
            return -1;
        }

        l_position += l_ObjInfo.size;
        //DEBUGPRINT("CURRENT POSITION SIZE %d\n", l_position);		
    }

    if(!m_drawerConn || m_drawerConn->send(msgBuffer, l_responseMsgSize) == -1)
    {
        printf("Failed to send!\n");
        DEBUGPRINT("failed to send");
        return -1;
    }

    DEBUGPRINT("Sent Response\n");
    return 0;	

    /*
    double elapsed = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
    cout << framestep << ": " << elapsed << "ms" << endl;
    */
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

