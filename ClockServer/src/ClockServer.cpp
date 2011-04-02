#include "ClockServer.h"
#include "Messages.h"
#include "Packer.h"
#include <string.h>
#include "Config.h"
#include <networkCommon.h>
#include <iostream>

Network::ClockServer::ClockServer()
{
	m_heartbeat = 0;
	m_clientList.empty();
	m_clientMap.empty();
	m_beat = 0;
	m_responded = 0;
	m_clockConn = NULL;
	m_ready = false;
	m_timesteps = 0;
	m_numGrids = 0;
}

Network::ClockServer::~ClockServer()
{

}

void Network::ClockServer::setNumGrids(int grids)
{
	m_numGrids = grids;
}

int Network::ClockServer::handler(int fd)
{
	//Create a 'header' message and buffer to receive into.
	Msg_header l_Header;
	Msg_HB l_heartBeat;
	unsigned char l_Buffer[l_Header.size + l_heartBeat.size];

	memset(&l_Buffer, 0, l_Header.size+l_heartBeat.size);

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_Clients[fd];
        
    if (l_Conn == NULL)
    {
    	ERRORPRINT("CLOCK_SERVER ERROR:\t No vaild connection\n");
    	return -1;
    }
    
    //Receive from the socket into our buffer.
	DEBUGPRINT("CLOCK_SERVER STATUS:\t About to receive header\n");
	
	if (l_Conn->recv(l_Buffer, l_Header.size) < 0)
	{
		ERRORPRINT("CLOCK_SERVER ERROR:\t Could not receive header\n");
		return -1;
	}

    unpack(l_Buffer, Msg_header_format, &l_Header.sender, &l_Header.message); 
    
	DEBUGPRINT("CLOCK_SERVER STATUS:\t Sender:%d Message:%d\n", l_Header.sender, l_Header.message);
	
    switch(l_Header.sender)
    {
        //Message is from clock.
        case(SENDER_CLIENT):
		{
            switch(l_Header.message)
            {
                //Message is heart beat.
                case(MSG_HEARTBEAT) :
                {
					DEBUGPRINT("CLOCK_SERVER STATUS:\t Receiving heartbeat from robot client or grid server\n");
					
					if (l_Conn->recv(l_Buffer+l_Header.size, l_heartBeat.size) == -1)
					{
						ERRORPRINT("CLOCK_SERVER ERROR:\t Could not receive heartbeat confirmation\n");
						return -1;
					}
					
					DEBUGPRINT("CLOCK_SERVER ERROR:\t Unpack the heartbeat\n");
					
					unpack(l_Buffer+l_Header.size, Msg_HB_format, &l_heartBeat.hb);
					
					DEBUGPRINT("CLOCK_SERVER ERROR:\t Heartbeat character is %d\n", l_heartBeat.hb);
					
                    if (m_beat == l_heartBeat.hb && m_clientMap[fd] == true)
					{
						m_responded++;
						m_clientMap[fd] = false;
					}
					
					if (m_responded == m_servers_total-1) SendHeartBeat();
                }
            }
            break;
		}
		break;
		case(SENDER_CONTROLLER):
		{
			switch(l_Header.message)
			{
				DEBUGPRINT("CLOCK_SERVER STATUS:\t Processing Message from Controller\n");
				case(MSG_CLOCKPROCEED):
				{
					DEBUGPRINT("CLOCK_SERVER STATUS:\t READY TO PROCEED msg from controller\n");
					SendHeartBeat();
				}
				break;
			}
		}
		break;
		default:
		{
			
		}
    }

	
    return 0;
}

int Network::ClockServer::handleNewConnection(int fd)
{
    DEBUGPRINT("clients %d numgrids %d\n", m_Clients.size(), m_numGrids);
	if (m_clientList.size() < m_numGrids)
	{
		LOGPRINT("CLOCK_SERVER STATUS:\t Adding new GRID_SERVER: %i\n", fd);
		m_clientMap[fd] = false;
		m_clientList.push_back(fd);
	} else if (m_ready)
	{
		LOGPRINT("CLOCK_SERVER STATUS:\t Adding new ROBOT_CLIENT %i\n", fd);
		m_clientMap[fd] = false;
		m_clientList.push_back(fd);
	} else {
		LOGPRINT("CLOCK_SERVER STATUS:\t Controller Connected\n");
		m_clockConn = m_Clients[0];
		m_ready = true;
	}
	return 0;
}

int Network::ClockServer::allConnectionReadyHandler()
{
	LOGPRINT("CLOCK_SERVER STATUS:\t CONTROLLER_CLIENT + all ROBOT_CLIENTS and GRID_SERVERS connected\n");
	return 0;
	
}

int Network::ClockServer::SendHeartBeat()
{
   	m_responded = 0;
   	int fd;
	DEBUGPRINT("CLOCK_SERVER STATUS:\t All ROBOT_CLIENTS and GRID_SERVERS ready\n");
	
	std::vector<int>::const_iterator l_End = m_clientList.end();
	
	TcpConnection * conn;
	
	Msg_header l_header;
	Msg_HB l_heartBeat;
	
	unsigned char msg[l_header.size + l_heartBeat.size];
	
	l_header.sender = SENDER_CLOCK;
	l_header.message = MSG_HEARTBEAT;
	
	if (m_beat == 1)
	{
		m_beat = 0;
	} else {
		m_beat = 1;
	}
	
	DEBUGPRINT("CLOCK_SERVER STATUS:\t Going to send BEAT:%u\n", m_beat);
	
	l_heartBeat.hb = m_beat;
	
	DEBUGPRINT("CLOCK_SERVER STATUS:\t Total Clients + Grid Servers to send BEAT: %zu\n",m_clientList.size());

	for(std::vector<int>::const_iterator it = m_clientList.begin(); it != l_End; it++)
	{
		DEBUGPRINT("CLOCK_SERVER STATUS:\t Prepairing heartbeat\n");

		fd = (*it);
		conn = m_Clients[fd];
		
		if (conn == NULL) 
		{
			ERRORPRINT("CLOCK_SERVER ERROR:\t Invalid Connection to ROBOT_CLIENT or GRID_SERVER\n");
			return -1;
		}
		int l_packed ;
		
		if ((l_packed = pack(msg, "hhh", l_header.sender, l_header.message, l_heartBeat.hb )) != l_header.size+l_heartBeat.size)
		{
			ERRORPRINT("CLOCK_SERVER ERROR:\t Failed to pack heart beat and message\n");
			return -1; // didn't pack all bytes FAIL & ABORT!!
		}

		if (conn->send(msg, l_header.size+l_heartBeat.size) < 0)
		{
			ERRORPRINT("CLOCK_SERVER ERROR:\t Failed to send message to ROBOT_CLIENT or GRID_SERVER\n");
		}
		
		DEBUGPRINT("CLOCK_SERVER STATUS:\t Sent heartbeat %hd to ROBOT_CLIENT or GRID_SERVER\n", l_heartBeat.hb);
		m_clientMap[fd] = true;
	}
	
	
	return 0;
}
