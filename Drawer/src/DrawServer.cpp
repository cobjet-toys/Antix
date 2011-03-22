#include "DrawServer.h"
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
	l_DrawerConfig.tl_x = topleft_x;
	l_DrawerConfig.tl_y = topleft_y;
	l_DrawerConfig.br_x = bottomright_x;
	l_DrawerConfig.br_y = bottomright_y;
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
	//if (newInfo == NULL) return;
	
    Math::Position *pos = new Math::Position(newInfo.x_pos, newInfo.x_pos, newInfo.angle);
    if (!pos)
    { 
    	DEBUGPRINT("Object %d: Invalid position\n", newInfo.robotid);
    	return;
    }
    
    uint32_t objType, objId;
    Antix::getTypeAndId(newInfo.robotid, &objType, &objId);
    if(objType == PUCK)
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
    		if (!this->m_teams[0])//newInfo.id/TEAM_ID_SHIFT])    
    		{
    			  DEBUGPRINT("No home for team %d\n", 1);//newInfo.id/TEAM_ID_SHIFT);
    			  return;
    		}         			
		    this->m_robots[objId] = new Game::Robot(pos, this->m_teams[0]->m_TeamId, objId);//newInfo.id/TEAM_ID_SHIFT]->getHome());
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
                    //Receive the total number of robots we are getting sens info for.
                    Msg_MsgSize l_NumObjects;
					NetworkCommon::recvMessageSize(l_NumObjects, l_Conn);
                    DEBUGPRINT("%d:\tExpecting %d objects.\n", this->m_framestep, l_NumObjects.msgSize );

                    
                    //Go through all of the objects we expect position data for.
                    for (int i = 0; i < l_NumObjects.msgSize;i++)
                    {
                        DEBUGPRINT("1\n");
		                Msg_RobotInfo l_ObjInfo;
		                unsigned char l_ObjInfoBuf[l_ObjInfo.size];
                        bzero(&l_ObjInfo, l_ObjInfo.size);
                        DEBUGPRINT("2\n");

                        recvWrapper(l_Conn, l_ObjInfoBuf, l_ObjInfo.size);
                        DEBUGPRINT("3\n");

                        unpack(l_ObjInfoBuf, Msg_RobotInfo_format,
                                &l_ObjInfo.robotid, &l_ObjInfo.x_pos, &l_ObjInfo.y_pos, &l_ObjInfo.angle, &l_ObjInfo.speed, &l_ObjInfo.puckid );

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
					return 0;
                }
                
                case (MSG_GRIDTEAMS) :
                {
                	//Receive the total number of teams we are getting info for.
                    Msg_MsgSize l_NumObjects;
					NetworkCommon::recvMessageSize(l_NumObjects, l_Conn);
                    DEBUGPRINT("MSG_GRIDTEAMS: Expecting %d teams.\n", l_NumObjects.msgSize );

                    //Go through all of the objects we expect data for.
                    for (int i = 0; i < l_NumObjects.msgSize;i++)
                    {
		                Msg_TeamInit l_TeamInfo;
		                unsigned char l_TeamInfoBuf[l_TeamInfo.size];
                        bzero(&l_TeamInfo, l_TeamInfo.size);
                        
                        recvWrapper(l_Conn, l_TeamInfoBuf, l_TeamInfo.size);
                        
                        unpack(l_TeamInfoBuf, Msg_TeamInit_format,
                                &l_TeamInfo.id, &l_TeamInfo.x, &l_TeamInfo.y);

						if (!this->m_teams[l_TeamInfo.id])
						{
							Math::Position *homePos = new Math::Position(l_TeamInfo.x, l_TeamInfo.y, 0.0);
		    				this->m_teams[l_TeamInfo.id] = new Game::Team(homePos, l_TeamInfo.id);
	    				}
    				}
    				
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



