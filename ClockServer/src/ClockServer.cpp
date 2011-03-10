#include "ClockServer.h"
#include "Messages.h"
#include "Packer.h"
#include <string.h>

Network::ClockServer::ClockServer()
{
	m_heartbeat = 0;
	m_clientList.empty();
	m_clientMap.empty();
}

Network::ClockServer::~ClockServer()
{

}

int Network::ClockServer::handler(int fd)
{
	printf("handle %i\n", fd);
    return 0;
}

int Network::ClockServer::handleNewConnection(int fd)
{
	printf("Adding new Client: %i\n", fd);
	m_clientMap[fd] = false;
	m_clientList.push_back(fd);
	return 0;
}

int Network::ClockServer::allConnectionReadyHandler()
{
	printf("All Clients ready\n");
	
	std::vector<int>::const_iterator l_End = m_clientList.end();
	
	int fd;
	int i;
	
	TcpConnection * conn;
	
	Msg_header l_header;
	Msg_HB l_heartBeat;
	
	unsigned char msg[l_header.size];
	
	l_header.sender = SENDER_CLOCK;
	l_header.message = MSG_HEARTBEAT;
	l_heartBeat.hb = 1;
	
	printf("Total Clients to send Hb: %i\n",m_clientList.size());
	
	for(std::vector<int>::const_iterator it = m_clientList.begin(); it != l_End; it++)
	{
		printf("Prepairing heartbeat\n");
		i = 0;
		fd = (*it);
		conn = m_Clients[fd];
		
		if (conn == NULL) 
		{
			printf("Conn = NULL\n");
			return -1;
		}
		
		if (pack(msg, Msg_header_format, l_header.sender, l_header.message) != l_header.size)
		{
			return -1; // didn't pack all bytes FAIL & ABORT!!
		}
		
		for (i = 0; i < 10000; i++)
		{
			if (conn->send(msg, l_header.size) == 0) break;
		}
		
		if (i == 100) 
		{
			printf("init failed to send header\n");
			return -1; // failed to send msg
		}
		
		if (pack(msg, Msg_HB_format, l_heartBeat.hb) != l_heartBeat.size)
		{
			return -1; // didn't pack all bytes FAIL & ABORT!!
		}
		
		printf("%i", l_heartBeat.size);
		
		for (i = 0; i < 10000; i++)
		{
			if (conn->send(msg, l_heartBeat.size) == 0) break;
		}
		
		if (i == 100) 
		{
			printf("init failed to send msg\n");
			return -1; // failed to send msg
		}
		
		printf("Sent heartbeat %hd\n", l_heartBeat.hb);
	}
	
	
	return 0;
}