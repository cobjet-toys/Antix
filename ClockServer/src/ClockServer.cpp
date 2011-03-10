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
	m_clientMap[fd] = false;
	m_clientList.push_back(fd);
	return 0;
}

int Network::ClockServer::allConnectionReadyHandler()
{
	vector<int>::const_iterator l_End = m_clientList.end();
	int fd;
	int i;
	TcpConnection * conn;
	Msg_header l_header;
	Msg_HB heartBeat;
	unsigned char msg[2];
	
	l_header.sender = SENDER_CLOCK;
	l_header.message = MSG_HEARTBEAT;
	
	for(vector<int>::const_iterator it = m_clientList.begin(); it != l_End; it++)
	{
		i = 0;
		fd = *(it);
		conn = m_clientMap[fd];
		
		if (conn == NULL) return -1;
		
		if (pack(&msg, l_header.format, l_header.size) != l_header.size)
		{
			return -1; // didn't pack all bytes FAIL & ABORT!!
		}
		
		for (i = 0; i < 100; i++)
		{
			if (conn->send(&msg, l_header.size) == 0) break;
		}
		
		if (i == 100) return -1; // failed to send msg
		
	}
	
	
	return 0;
}