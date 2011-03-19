#include "GridServer.h"
#include "Config.h"
#include "Types.h"
#include <map>
#include <vector>
#include "networkCommon.h"

using namespace Network;

GridServer::GridServer():Server()
{
    m_uId = -1;
    m_hb_rcvd = 0;
}
int GridServer::handleNewConnection(int fd)
{
    return 0;
}

int GridServer::allConnectionReadyHandler()
{
    return 0;
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
						s1.robotid = i;
						s1.x = i;
						s1.y = i;
						a1.push_back(s1);
					}
					DEBUGPRINT("got all sensory data\n");
					for (int i = 0; i < l_robotsTotal; i++)
					{
						sensed_items_map[i] = a1;
					}
					DEBUGPRINT("got all robot <= sensory data total of %d robots with %d sensory objects\n", a1.size(),sensed_items_map.size());
					
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
							l_sensedObject.robotid = vecIt->robotid;
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
				
				case (MSG_PROCESSINITTEAM):
				{					
					Msg_header l_header;
					Msg_MsgSize l_msgSize;
					
					NetworkCommon::requestMessageSize(l_msgSize, l_curConnection);
					
					int l_numTeams = l_msgSize.msgSize;
					
					if (l_numTeams <= 0)
					{
						DEBUGPRINT("Requested sensor data for <=0 Robots. Game error.\n");
						return -1;
					}
					
					DEBUGPRINT("Client has requested actions for %i robots\n", l_numTeams);
					
					Msg_TeamId l_teamId;
					unsigned char l_teamIdBuff[l_teamId.size];
					
					memset(&l_teamId, 0 , l_teamId.size);
					memset(l_teamIdBuff, 0 , l_teamId.size);
					
					std::vector<int> l_teamIdsVec;
					std::map<int, std::vector<robot_info> > l_teamRobotsMap;
					
					for (int i = 0; i < l_numTeams; i++)
					{
						memset(l_teamIdBuff, 0 , l_teamId.size);
						if (l_curConnection->recv(l_teamIdBuff, l_teamId.size) == -1)
						{
							DEBUGPRINT("Could not receive a team id.");
							return -1;
						}
						unpack(l_teamIdBuff, Msg_TeamId_format, &l_teamId.teamId);
						
						l_teamIdsVec.push_back(l_teamId.teamId);
						
						DEBUGPRINT("Team id:%d unpacked and stored\n", l_teamId.teamId);
					}
					
					/*if (GridGame::initializeTeam(vector<int> teamids, map<int, vector<robot_info> >* robots) < 0)
					{
						DEBUGPRINT("Initialize Team\n");
						return -1;
					}*/
					
					return 0;
				}
				
				default:
				{
					DEBUGPRINT("No matching message handle for client case.\n");
					return -1;
				}
			} // end of sender
		default:
		{
			printf("no matching msg handler for UNKNOWN\n");
			return -1;
		} // end of default
		
	} // end of switch

    return 0;
}

void GridServer::initDrawer(const char* drawer_host, const char* drawer_port)
{
	int l_drawer_fd = initDrawerConnection(drawer_host, drawer_port);
	int home = 1;
	
	//send team data    

	/*
    for(int i=0; i<homes; i++)
    {
        int team_id = i+1;
        float posX = 55.0;
        float posY = 150.0;
        int colR = 255;
        int colG = 255;
        int colB = 255;
        char buf[64];
        bzero(buf, 64);
        sprintf(buf, TEAM_PATTERN, team_id, posX, posY, colR, colG, colB);
        cout << buf << endl;
        this->posDB->append(string(buf));
    }
	*/
}

int GridServer::updateDrawer(uint32_t framestep)
{
	printf("----updateDrawer---------\n");
		
	int m_totalRobots = 1000;
	int m_totalPucks = 5;
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
	memset(&l_msgSize, 0 , l_msgSize.size);	
	l_msgSize.msgSize = 10000; 	//robots + pucks
	
	Msg_RobotInfo l_ObjInfo;	//for each object
	
	unsigned int l_responseMsgSize = 0;	
	l_responseMsgSize += l_header.size; 					// append header size	
	l_responseMsgSize += l_msgSize.size;					//append msgSize size
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
	
	DEBUGPRINT("AFTER PACKING SIZE %d\n", l_position);
					
	for (int i=0; i< m_totalRobots; i++)
	{
        // Computes robots altered position (Random)
        robotPosition[i][0] += float((rand()%200)-100)/50;
        robotPosition[i][1] += float((rand()%200)-100)/50;

        float posX = robotPosition[i][0];//(float)i*10+j*5;
        float posY = robotPosition[i][1];//i*15+j*3;
        float orientation = 1.0;
		
		// for each object being pushed
		l_ObjInfo.id = i + 1024;
		l_ObjInfo.x_pos = posX;
		l_ObjInfo.y_pos = posY;
		l_ObjInfo.angle = orientation;
		l_ObjInfo.has_puck = i%2 == 0 ? 'T' : 'F';
		
		DEBUGPRINT("Object: newInfo.id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%c\n",
                                l_ObjInfo.id, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.has_puck );
                         
		if (pack(msgBuffer+l_position, Msg_RobotInfo_format,
                                &l_ObjInfo.id, &l_ObjInfo.x_pos, &l_ObjInfo.y_pos, &l_ObjInfo.angle, &l_ObjInfo.has_puck ) != l_ObjInfo.size)
		{
			DEBUGPRINT("Could not pack robot header\n");
			return -1;
		}
		
		l_position += l_ObjInfo.size;
		DEBUGPRINT("CURRENT POSITION SIZE %d\n", l_position);		
	}
					
	for (int i=0; i< m_totalPucks; i++)
	{
        float posX = randPuckVals[(2*i)];
        float posY = randPuckVals[(2*i) + 1];
		
		// for each object being pushed
		l_ObjInfo.id = i;
		l_ObjInfo.x_pos = posX;
		l_ObjInfo.y_pos = posY;
		l_ObjInfo.angle = 1.0;
		l_ObjInfo.has_puck = 'F';
		
		DEBUGPRINT("Object: newInfo.id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%c\n",
                                l_ObjInfo.id, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.has_puck );
                         
		if (pack(msgBuffer+l_position, Msg_RobotInfo_format,
                                &l_ObjInfo.id, &l_ObjInfo.x_pos, &l_ObjInfo.y_pos, &l_ObjInfo.angle, &l_ObjInfo.has_puck) != l_ObjInfo.size)
		{
			DEBUGPRINT("Could not pack robot header\n");
			return -1;
		}
		
		l_position += l_ObjInfo.size;
		DEBUGPRINT("CURRENT POSITION SIZE %d\n", l_position);		
	}
	
	if (m_drawerConn->send(msgBuffer, l_responseMsgSize) == -1)
	{
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

int GridServer::initDrawerConnection(const char* host, const char* port)
{
	if (strlen(host) > INET6_ADDRSTRLEN || strlen(port) > MAX_PORT_LENGTH )
	{
		if (DEBUG) perror("Invalid port or host length to init");
		return -2;
	}
	
	if (host == NULL || port == NULL)
	{
		if (DEBUG) perror("Invalid arguments to init");
	} 
	
	addrinfo l_hints, *l_result, *l_p;
	int l_resvalue = -1;
	
	memset(&l_hints, 0, sizeof(struct addrinfo) );
	
	l_hints.ai_family = AF_UNSPEC;
	l_hints.ai_socktype = SOCK_STREAM;
	
	if ((l_resvalue = getaddrinfo(host, port, &l_hints, &l_result)) == -1)
	{
		if (DEBUG) fprintf(stderr, "Could not get addrinfo: %s\n", gai_strerror(l_resvalue));
		return -1;
	}
	
	m_drawerConn = new TcpConnection();
	
	if (m_drawerConn == NULL)
	{
		if (DEBUG) printf("Could not create new TcpConnection\n");
		return -1;
	}

	for (l_p = l_result; l_p != NULL; l_p = l_p->ai_next)
	{
		if ((l_resvalue =m_drawerConn->socket(l_p)) != 0)
		{
			if (l_resvalue == -1) 
			{
				if (DEBUG) perror("Could not connect socket trying next\n");
			} else if (l_resvalue == -2)
			{
				if (DEBUG) perror("Argument error to socket\n");
			} else 
			{
				if (DEBUG) perror("What did I just return!?!?\n");
			}
			continue;
		}
		
		if ((l_resvalue = m_drawerConn->connect(l_p)) != 0)
		{
			if (l_resvalue == -1)
			{
				if (DEBUG) perror("Could not connect to server/port\n");
			} else if (l_resvalue == -2)
			{
				if (DEBUG) perror("Argument error to connect\n");
			} else {
				if (DEBUG) perror("What did I just return!?!?\n");
			}
			m_drawerConn->close();
			continue;
		}
		
		break;
	}
	
	if (l_p == NULL)
	{
		if (DEBUG) perror("No valid addrinfo");
		return -1;
	}
	
	int fileDesc = m_drawerConn->getSocketFd();
	
	/*
	if ( setnonblock(fileDesc) == -1)
	{
		if (DEBUG) printf("Could not set socket to non-blocking\n");
	}
	
	if( addHandler(fileDesc, EPOLLET|EPOLLIN|EPOLLHUP|EPOLLPRI, m_drawerConn ) == -1)
	{
		if (DEBUG) printf("Could not add handler to tcpConnection");
		return -1;
	}
	*/
	
	return fileDesc;

}

