#include "DrawerClient.h"
#include "AntixUtil.h"

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

using namespace Network;

DrawServer* DrawServer::m_instance = NULL;

DrawServer::DrawServer()
{    
    this->m_windowSize = 600;
    this->m_worldSize = 1.0;
    this->m_FOVEnabled = false;
    this->m_FOVAngle = 0.0;
    this->m_FOVRange = 0.0;
    this->m_homeRadius = 20.0;
    this->m_framestep = 0;
    this->m_drawerDataType = DRAWER_FULLDETAILS;    
    
	this->m_viewTL_x = 0;
	this->m_viewTL_y = 0;
	this->m_viewBR_x = this->m_windowSize;
	this->m_viewBR_y = this->m_windowSize;
}

DrawServer::~DrawServer() 
{
	for(int i=0; i<this->m_pucks.size(); i++)
	{
		delete this->m_pucks[i];
	}
	this->m_pucks.clear();
	
	
	for(int i=0; i<this->m_robots.size(); i++)
	{
		delete this->m_robots[i];
	}
	this->m_pucks.clear();
	
	
	for(int i=0; i<this->m_teams.size(); i++)
	{
		delete this->m_teams[i];
	}
	this->m_teams.clear();

}

DrawServer* DrawServer::getInstance()
{
    if (!m_instance)
    {    
        m_instance = new DrawServer();
    }

    return m_instance;
}

void DrawServer::setWindowSize(int val)
{
	this->m_windowSize = val;
	this->m_viewBR_x = this->m_windowSize;
	this->m_viewBR_y = this->m_windowSize;
}

// Sends initialization message to grid @ host:port, telling it 
// to send info about its known pucks and robots
int DrawServer::initGrid(const char* host, const char* port, int id)
{
    int l_GridFd = initConnection(host, port);

    if (l_GridFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
    }
    
    sendGridConfig(l_GridFd);
    return l_GridFd;
}

// Fills local collection of robots with correct number of robots
// per team, starting off view (-1,-1)
size_t DrawServer::initRobots(int size)
{	
	int teamId = 0;
    Math::Position *pos = new Math::Position(-1.0, -1.0, 0.0);
    this->m_robots.clear();
    
    for (int i=0; i<size; i++)
    {
    	this->m_robots.push_back( new Game::Robot(pos, teamId, i) );
    	
    	if (i % this->m_totalNumTeams == 0) teamId++;
    }
    return this->m_robots.size();
}

// Fills local collection of robots with correct number of pucks
// starting off view (-1,-1)
size_t DrawServer::initPucks(int size)
{	
    Math::Position *pos = new Math::Position(-1.0, -1.0, 0.0);
    this->m_pucks.clear();
    
    for (int i=0; i<size; i++)
    {
    	this->m_pucks.push_back( new Game::Puck(pos) );
    }
    
    return this->m_pucks.size();
}

void DrawServer::updateViewRange(float tl_x, float tl_y, float br_x, float br_y)
{	
	this->m_viewTL_x = tl_x;
	this->m_viewTL_y = tl_y;
	this->m_viewBR_x = br_x;
	this->m_viewBR_y = br_y;	
	
	std::map<int, TcpConnection*>::iterator it;	
	
	for (it = this->m_serverList.begin(); it != this->m_serverList.end(); it++)
	{
		this->sendGridConfig((*it).first);
	}
}

int DrawServer::sendGridConfig(int grid_fd)
{
	//send config to grid
    TcpConnection * l_curConn = m_serverList[grid_fd];
    if (l_curConn == 0)
    	return -1;
    
     //Header message;
    Msg_header l_Header;

    //Drawer config;
    Msg_DrawerConfig l_DrawerConfig;

    //Create our message buffer
    unsigned int l_MessageSize = l_Header.size + l_DrawerConfig.size;
    unsigned char l_Buffer[l_MessageSize]; 
    
    //Pack msg header
    unsigned int l_BufferOf = 0;
    packHeaderMessage(l_Buffer+l_BufferOf, SENDER_DRAWER, MSG_SETDRAWERCONFIG);
    l_BufferOf += l_Header.size;
    
    //Pack config message
	l_DrawerConfig.send_data = 'T';
	l_DrawerConfig.data_type = this->m_drawerDataType;
	l_DrawerConfig.tl_x = this->m_viewTL_x;
	l_DrawerConfig.tl_y = this->m_viewTL_y;
	l_DrawerConfig.br_x = this->m_viewBR_x;
	l_DrawerConfig.br_y = this->m_viewBR_y;
    pack(l_Buffer+l_BufferOf, Msg_DrawerConfig_format, 
    	l_DrawerConfig.send_data, l_DrawerConfig.data_type, l_DrawerConfig.tl_x, l_DrawerConfig.tl_y, l_DrawerConfig.br_x, l_DrawerConfig.br_y);
    
    
    return sendWrapper(l_curConn, l_Buffer, l_MessageSize);   
}

void DrawServer::initTeams()
{    
    if (this->m_teams.size() == 0)
    {
    	Math::Position *homePos = new Math::Position(55.0, 150.0, 0.0);
        this->m_teams[1] = new Game::Team(homePos, 1);
    }

    DEBUGPRINT("Teams=%zu\n", this->m_teams.size());
}

void DrawServer::updateObject(Msg_RobotInfo newInfo)
{    
    uint32_t objType, objId;
    Antix::getTypeAndId(newInfo.robotid, &objType, &objId);
    if(objType == PUCK)
    {
        this->m_pucks.at(objId)->getPosition()->setX(newInfo.x_pos);
		this->m_pucks.at(objId)->getPosition()->setY(newInfo.y_pos);        		
    }
    else
    {    
    	
    	this->m_robots.at(objId)->getPosition()->setX(newInfo.x_pos);
		this->m_robots.at(objId)->getPosition()->setY(newInfo.y_pos);       
		
		/*
      	if (!this->m_robots[objId])
      	{
    		if (!this->m_teams[0])//newInfo.id/TEAM_ID_SHIFT])    
    		{
    			  DEBUGPRINT("No home for team %d\n", 1);//newInfo.id/TEAM_ID_SHIFT);
    			  return;
    		}         			
		    this->m_robots[objId] = new Game::Robot(pos, this->m_teams[0]->m_TeamId, objId);//newInfo.id/TEAM_ID_SHIFT]->getHome());
      	}     
      	*/
        
        //this->m_robots[newInfo.id]->m_PuckHeld = this->m_pucks[newInfo.puck_id];
    }
}


//SERVER METHODS

int DrawServer::handler(int fd)
{
    //DEBUGPRINT("Handling file descriptor: %i\n", fd);

    //Get our TCPConnection for this socket.    
    TcpConnection * l_Conn = this->m_serverList[fd];	
	if (l_Conn == NULL)
    {
		return -1; // no such socket descriptor
	}	
	
	//Get message header
	uint16_t l_sender=-1, l_senderMsg =-1;	
	NetworkCommon::recvHeader(l_sender, l_senderMsg, l_Conn);
	if (l_sender == -1 || l_senderMsg == -1) return -1;
		
    switch(l_sender)
    {
        case(SENDER_GRIDSERVER):
            switch(l_senderMsg)
            {
                case(MSG_GRIDDATAFULL) :
                {
                    DEBUGPRINT("MSG_GRIDDATAFULL\n");
                    
                    Msg_MsgSize l_NumObjects;
	                Msg_RobotInfo l_ObjInfo;
	                unsigned char l_ObjInfoBuf[l_ObjInfo.size];
	                
                    //Receive the total number of robots we are getting sens info for.
					NetworkCommon::recvMessageSize(l_NumObjects, l_Conn);
                    DEBUGPRINT("%d:\tExpecting %d objects.\n", this->m_framestep, l_NumObjects.msgSize );
                    
                    //Go through all of the objects we expect position data for.
                    for (int i = 0; i < l_NumObjects.msgSize;i++)
                    {
                        bzero(&l_ObjInfo, l_ObjInfo.size);
                        
                        DEBUGPRINT("1\n");
                        recvWrapper(l_Conn, l_ObjInfoBuf, l_ObjInfo.size);
                        DEBUGPRINT("2\n");

                        unpack(l_ObjInfoBuf, Msg_RobotInfo_format,
                                &l_ObjInfo.robotid, &l_ObjInfo.x_pos, &l_ObjInfo.y_pos, &l_ObjInfo.speed, &l_ObjInfo.angle, &l_ObjInfo.puckid );

                        DEBUGPRINT("Object: id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%d\n",
                        	        l_ObjInfo.robotid, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.puckid );
                                
                        updateObject(l_ObjInfo);
                    }
                    
                    DEBUGPRINT("%d:\trobots=%zu\tpucks=%zu.\n", this->m_framestep, this->m_robots.size(), this->m_pucks.size() );
					this->m_framestep++;
					return 0;
                }
                
                case(MSG_GRIDDATACOMPRESS) : 
                {                
                    DEBUGPRINT("MSG_GRIDDATACOMPRESS\n");
					return 0;
                }
                
                case (MSG_GRIDTEAMS) :
                {
                    Msg_MsgSize l_NumObjects;
	                Msg_TeamInit l_TeamInfo;
	                unsigned char l_TeamInfoBuf[l_TeamInfo.size];
	                
                	//Receive the total number of teams we are getting info for.
					NetworkCommon::recvMessageSize(l_NumObjects, l_Conn);
                    DEBUGPRINT("MSG_GRIDTEAMS: Expecting %d teams.\n", l_NumObjects.msgSize );

                    //Go through all of the objects we expect data for.
                    for (int i = 0; i < l_NumObjects.msgSize;i++)
                    {
                        bzero(&l_TeamInfo, l_TeamInfo.size);
                        
                        recvWrapper(l_Conn, l_TeamInfoBuf, l_TeamInfo.size);
                        
                        unpack(l_TeamInfoBuf, Msg_TeamInit_format,
                                &l_TeamInfo.id, &l_TeamInfo.x, &l_TeamInfo.y);
                                
                        DEBUGPRINT("Team: id=%d\tx=%f\ty=%f\n",
                	        l_TeamInfo.id, l_TeamInfo.x, l_TeamInfo.y );
                        

						//TODO: check to make sure team doesn't already exist
						{
							Math::Position *homePos = new Math::Position(l_TeamInfo.x, l_TeamInfo.y, 0.0);
		    				Game::Team *newTeam = new Game::Team(homePos, l_TeamInfo.id);
		    				this->m_teams.push_back(newTeam);
	    				}
    				}
    				
                    DEBUGPRINT("MSG_GRIDTEAMS: Finished.\n");
    				
					return 0;
                }
            }
            break;
    }
    
    return 0;
}






// NETWORK PACKING METHODS


int DrawServer::sendWrapper(TcpConnection * conn, unsigned char* buffer, int msgSize)
{
    if (conn->send(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to send a message.\n");
        return -1;
    }
    return 0;
}

int DrawServer::recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize)
{
    if (conn->recv(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to receive a message.\n");
        return -1;
    }
    return 0;
}

int DrawServer::packHeaderMessage(unsigned char* buffer, uint16_t sender, uint16_t message)
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



