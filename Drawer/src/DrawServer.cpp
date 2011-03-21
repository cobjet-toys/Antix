#include "DrawServer.h"
#include "Home.h"
#include "Robot.h"
#include "Puck.h"
#include "Team.h"
#include "Position.h"

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
}

DrawServer::~DrawServer() 
{
}

DrawServer* DrawServer::getInstance()
{
    if (!m_instance)
    {    
        m_instance = new DrawServer();
    }

    return m_instance;
}

/*
int DrawServer::init(int argc, char** argv)
{
	if (Client::init() < 0)
		return -1;

    if (argc > 2)
    {
        //<window_size> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ]
        this->m_windowSize = atoi(argv[2]);
        this->m_worldSize = strtof(argv[3], NULL);
        this->m_homeRadius = strtof(argv[4], NULL);
        this->m_FOVEnabled = atoi(argv[5]);
        if (this->m_FOVEnabled)
        {
            this->m_FOVAngle = strtof(argv[6], NULL);
            this->m_FOVRange = strtof(argv[7], NULL);
        }
    }   
    
    return 0;
}
*/

int DrawServer::initGrid(const char* host, const char* port, int id)
{
    int l_GridFd = initConnection(host, port);

    if (l_GridFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
    }
    
    setGridConfig(l_GridFd, 'T');
    return l_GridFd;
}

int DrawServer::setGridConfig(int grid_fd, char send_data, float topleft_x, float topleft_y, float bottomright_x, float bottomright_y)
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
	l_DrawerConfig.send_data = send_data;
	l_DrawerConfig.data_type = this->m_drawerDataType;
	l_DrawerConfig.left_x = topleft_x;
	l_DrawerConfig.left_y = topleft_y;
	l_DrawerConfig.right_x = bottomright_x;
	l_DrawerConfig.right_y = bottomright_y;
    pack(l_Buffer+l_BufferOf, Msg_DrawerConfig_format, 
    	l_DrawerConfig.send_data, l_DrawerConfig.data_type, l_DrawerConfig.left_x, l_DrawerConfig.left_y, l_DrawerConfig.right_x, l_DrawerConfig.right_y);
    
    
    return sendWrapper(l_curConn, l_Buffer, l_MessageSize);   
}

void DrawServer::initTeams()
{    
    if (this->m_teams.size() == 0)
    {
    	Math::Position *homePos = new Math::Position(55.0, 150.0, 0.0);
        Game::Home * home = new Game::Home(homePos);        
        this->m_teams[1] = new Game::Team();
        this->m_teams[1]->m_Home = home;
    }

    DEBUGPRINT("Teams=%d\n", this->m_teams.size());
}

void DrawServer::updateObject(Msg_RobotInfo newInfo)
{
	//if (newInfo == NULL) return;
	
    Math::Position *pos = new Math::Position(newInfo.x_pos, newInfo.x_pos, newInfo.angle);
    if (!pos)
    { 
    	DEBUGPRINT("Object %d: Invalid position\n", newInfo.id);
    	return;
    }
    
    int objId = newInfo.id % TEAM_ID_SHIFT;
    DEBUGPRINT("%d = %d %% %d\n", objId, newInfo.id, TEAM_ID_SHIFT);
    if(objId == 0)
    {
        if (!this->m_pucks[objId])
        {
        	  this->m_pucks[objId] = new Game::Puck(pos);
        }
      	else
      	{
      		  this->m_pucks[objId]->getPosition()->setX(newInfo.x_pos);
      		  this->m_pucks[objId]->getPosition()->setY(newInfo.y_pos);        		
      	}
    }
    else
    {
      	if (!this->m_robots[objId])
      	{
        		if (!this->m_teams[1])//newInfo.id/TEAM_ID_SHIFT])    
        		{
        			  DEBUGPRINT("No home for team %d\n", 1);//newInfo.id/TEAM_ID_SHIFT);
        			  return;
        		}         			
  			    this->m_robots[objId] = new Game::Robot(pos, this->m_teams[1]->getHome());//newInfo.id/TEAM_ID_SHIFT]->getHome());
      	}          	
      	else
      	{
        		this->m_robots[objId]->getPosition()->setX(newInfo.x_pos);
        		this->m_robots[objId]->getPosition()->setY(newInfo.y_pos);        		
      	}
        
        //this->m_robots[newInfo.id]->m_PuckHeld = this->m_pucks[newInfo.puck_id];
    }
}


//SERVER METHODS

int DrawServer::handler(int fd)
{
    DEBUGPRINT("Handling file descriptor: %i\n", fd);

    //Get our TCPConnection for this socket.    
    TcpConnection * l_Conn = this->m_serverList[fd];	
	if (l_Conn == NULL)
    {
		return -1; // no such socket descriptor
	}	
	
	//Get message header
	uint16_t l_sender=-1, l_senderMsg =-1;	
	NetworkCommon::requestHeader(l_sender, l_senderMsg, l_Conn);
	if (l_sender == -1 || l_senderMsg == -1) return -1;
		
    switch(l_sender)
    {
        case(SENDER_GRIDSERVER):
            switch(l_senderMsg)
            {
                case(MSG_GRIDDATAFULL) :
                {
                    //Receive the total number of robots we are getting sens info for.
                    Msg_MsgSize l_NumObjects;
					NetworkCommon::requestMessageSize(l_NumObjects, l_Conn);
                    DEBUGPRINT("Expecting %d objects.\n", l_NumObjects.msgSize );
                    
                    //containers to hold processed/received data

                    //Go through all of the objects we expect position data for.
                    for (int i = 0; i < l_NumObjects.msgSize;i++)
                    {
		                Msg_RobotInfo l_ObjInfo;
		                unsigned char l_ObjInfoBuf[l_ObjInfo.size];
                        bzero(&l_ObjInfo, l_ObjInfo.size);
                        
                        recvWrapper(l_Conn, l_ObjInfoBuf, l_ObjInfo.size);
                        
                        unpack(l_ObjInfoBuf, Msg_RobotInfo_format,
                                &l_ObjInfo.id, &l_ObjInfo.x_pos, &l_ObjInfo.y_pos, &l_ObjInfo.angle, &l_ObjInfo.has_puck );

                        //DEBUGPRINT("Object: newInfo.id=%d\tx=%f\ty=%f\tangle=%f\tpuck=%c\n",
                        //        l_ObjInfo.id, l_ObjInfo.x_pos, l_ObjInfo.y_pos, l_ObjInfo.angle, l_ObjInfo.has_puck );
                                
                        updateObject(l_ObjInfo);
                    }
                    //DEBUGPRINT("Finished updating objects\n");

                }
                
                case(MSG_GRIDDATACOMPRESS) : 
                {
                }
                
                //case team initialization info
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



