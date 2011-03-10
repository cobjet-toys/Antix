#include "RobotClient.h"

using namespace Network;

RobotClient::RobotClient():Client()
{
	m_heartbeat = 0;
}


int RobotClient::handler(int fd)
{
	printf("handeling fd %i\n", fd);
	//TcpConnection * l_curConnection = m_Clients[fd];
    while (1)
    {
    }    
    return 0;
}
