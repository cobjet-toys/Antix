#include "ClockServer.h"
#include "Messages.h"
#include "Packer.h"

Network::ClockServer::ClockServer()
{
	m_heartbeat = 0;
}

Network::ClockServer::~ClockServer()
{

}

int Network::ClockServer::handler(int fd)
{
	TcpConnection * conn = m_Clients[fd];
	
	
	
    return 0;
}
