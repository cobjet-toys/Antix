#include "DrawerClient.h"
#include "AntixUtil.h"

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define PUCK_ID_SHIFT 10000000

using namespace Network;

DrawServer* DrawServer::m_instance = NULL;

DrawServer::DrawServer()
{    
    this->m_windowSize = 600;
    this->m_worldSize = 1.0f;
    this->m_FOVEnabled = false;
    this->m_FOVAngle = 0.0f;
    this->m_FOVRange = 0.0f;
    this->m_homeRadius = 20.0f;
    this->m_framestep = 0;
    this->m_drawerDataType = DRAWER_FULLDETAILS;    
    
	this->m_viewLeft = 0;
	this->m_viewTop = 0;
	this->m_viewRight = this->m_windowSize;
	this->m_viewBottom = this->m_windowSize;
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
	this->m_viewRight = this->m_windowSize;
	this->m_viewBottom = this->m_windowSize;
}

// Sends initialization message to grid @ host:port, telling it 
// to send info about its known pucks and robots
int DrawServer::initGrid(const char* host, const char* port)
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
	Math::Position *pos = 0;
	Game::Robot *robot = 0;
    this->m_robots.clear();
    
    for (int i=0; i<size; i++)
    {
		pos = new Math::Position(-1.0, -1.0, 0.0);
		if(pos == NULL)
		{
			ERRORPRINT("DRAWSERVER ERROR:\t Failed to allocate memory for position in initRobots()\n");
			return -1;
		}
		robot = new Game::Robot(pos, i);
		if(robot == NULL)
		{
			ERRORPRINT("DRAWSERVER ERROR:\t Failed to allocate memory for robot in initRobots()\n");
			return -1;
		}
    	this->m_robots.push_back( robot );
    	
    	if (i % this->m_totalNumTeams == 0) teamId++;
    }
    return this->m_robots.size();
}

// Fills local collection of robots with correct number of pucks
// starting off view (-1,-1)
size_t DrawServer::initPucks(int size)
{	
    Math::Position *pos = 0;
    Game::Puck *puck = 0;
    this->m_pucks.clear();
    
    for (int i=0; i<size; i++)
    {    	
		pos = new Math::Position(-1.0, -1.0, 0.0);
		if(pos == NULL)
		{
			ERRORPRINT("DRAWSERVER ERROR:\t Failed to allocate memory for position in initPucks()\n");
			return -1;
		}
		puck = new Game::Puck(pos);
		if(puck == NULL)
		{
			ERRORPRINT("DRAWSERVER ERROR:\t Failed to allocate memory for puck in initPucks()\n");
			return -1;
		}
    	this->m_pucks.push_back( puck );
    }
    
    return this->m_pucks.size();
}

void DrawServer::updateViewRange(float left, float top, float right, float bottom)
{	
	this->m_viewLeft = left;
	this->m_viewTop = top;
	this->m_viewRight = right;
	this->m_viewBottom = bottom;	
	
	//reset puck and robot positions	
	for(int i=0; i<this->m_pucks.size(); i++)
	{
		this->m_pucks[i]->setPosition(-1.0, -1.0, 0);
	}		
	for(int i=0; i<this->m_robots.size(); i++)
	{
		this->m_robots[i]->setPosition(-1.0, -1.0, 0);
	}
	
	//notify grids
	std::map<int, TcpConnection*>::iterator it;		
	for (it = this->m_serverList.begin(); it != this->m_serverList.end(); it++)
	{
		this->sendGridConfig((*it).first);
	}
}

int DrawServer::sendGridConfig(int grid_fd)
{
	if (grid_fd < 0)
	{
		ERRORPRINT("DRAWSERVER ERROR:\t Grid fiile descriptor cannot be negative\n");
		return 0;
	}
	//send config to grid
    TcpConnection * l_curConn = m_serverList[grid_fd];
	if(l_curConn == NULL)
	{
		ERRORPRINT("DRAWSERVER ERROR:\t Failed to create connection in sendGridConfig()\n");
		return -1;
	}
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
	l_DrawerConfig.top = this->m_viewTop;
	l_DrawerConfig.bottom = this->m_viewBottom;
	l_DrawerConfig.right = this->m_viewRight;
	l_DrawerConfig.left = this->m_viewLeft;
    pack(l_Buffer+l_BufferOf, Msg_DrawerConfig_format, 
    	l_DrawerConfig.send_data, l_DrawerConfig.data_type, l_DrawerConfig.top, l_DrawerConfig.bottom, l_DrawerConfig.left, l_DrawerConfig.right);
    
    return sendWrapper(l_curConn, l_Buffer, l_MessageSize);   
}

void DrawServer::updateObject(Msg_DrawerObjectInfo newInfo)
{    
    uint32_t objType, objId, objIndex, puckIndex;
    Antix::getTypeAndId(newInfo.robotid, &objType, &objId);
	objIndex = objId - 1;
	objType = objId > PUCK_ID_SHIFT;
	
	try
	{
		if(objType == PUCK)
		{
			objIndex -= PUCK_ID_SHIFT;
		    this->m_pucks.at(objIndex)->setPosition(newInfo.x_pos, newInfo.y_pos, 0.0f);
			//DEBUGPRINT("Puck[%d]: x=%f, y=%f\n", objIndex, this->m_pucks.at(objIndex)->getPosition()->getX(), this->m_pucks.at(objIndex)->getPosition()->getY() );  		
		}
		else
		{    
			
			this->m_robots.at(objIndex)->setPosition(newInfo.x_pos, newInfo.y_pos, newInfo.angle);
		    this->m_robots.at(objIndex)->setPuckHeld(newInfo.puckid);
		    
		    puckIndex = newInfo.puckid - PUCK_ID_SHIFT;
		    if (puckIndex > 0 && this->m_pucks.size() > puckIndex)
		    	this->m_pucks.at(objIndex)->setPosition(-1.0f, -1.0f, 0.0f);
		    
		    DEBUGPRINT("Robot[%d]: x=%f, y=%f, puck=%d\n", objIndex, this->m_robots.at(objIndex)->getPosition()->getX(), this->m_robots.at(objIndex)->getPosition()->getY(), this->m_robots.at(objIndex)->getPuckHeld() );  		
		}
	}
	catch (std::exception &e)
	{
		DEBUGPRINT("%s doesn't exist at %d\n", (objType == PUCK) ? "Puck" : "Robot",  objIndex);
	}
}


//SERVER METHODS

int DrawServer::handler(int fd)
{
    //DEBUGPRINT("Handling file descriptor: %i\n", fd);

    //Get our TCPConnection for this socket.    
    TcpConnection * l_Conn = this->m_serverList[fd];
	if(l_Conn == NULL)
	{
		ERRORPRINT("DRAWSERVER ERROR:\t Failed to create connection in handler()\n");
		return -1;
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
	                Msg_DrawerObjectInfo l_ObjInfo;
	                unsigned char * l_ObjInfoBuf = new unsigned char[l_ObjInfo.size];
	                
                    //Receive the total number of robots we are getting sens info for.
					NetworkCommon::recvMessageSize(l_NumObjects, l_Conn);
                    DEBUGPRINT("%d:\tExpecting %d objects.\n", this->m_framestep, l_NumObjects.msgSize );
                    
                    //Go through all of the objects we expect position data for.
                    for (int i = 0; i < l_NumObjects.msgSize;i++)
                    {
                        bzero(&l_ObjInfo, l_ObjInfo.size);
                        
                        recvWrapper(l_Conn, l_ObjInfoBuf, l_ObjInfo.size);

                        unpack(l_ObjInfoBuf, Msg_DrawerObjectInfo_format,
                                &l_ObjInfo.robotid, &l_ObjInfo.x_pos, &l_ObjInfo.y_pos, &l_ObjInfo.angle, &l_ObjInfo.puckid );
                                
                        updateObject(l_ObjInfo);
                    }
                    
                    delete l_ObjInfoBuf;
                    
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
	                unsigned char * l_TeamInfoBuf = new unsigned char[l_TeamInfo.size];
	                
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
							if(homePos == NULL)
							{
								ERRORPRINT("DRAWSERVER ERROR:\t Failed to allocate memory for home position for team %d in handler()\n", l_TeamInfo.id);
								return -1;
							}
		    				Game::Team *newTeam = new Game::Team(homePos, l_TeamInfo.id);
							if(newTeam == NULL)
							{
								ERRORPRINT("DRAWSERVER ERROR:\t Failed to allocate memory for new team %d in handler()\n", l_TeamInfo.id);
								return -1;
							}
		    				this->m_teams.push_back(newTeam);
	    				}
    				}
    				
    				delete l_TeamInfoBuf;
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



