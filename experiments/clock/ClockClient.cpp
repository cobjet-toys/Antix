#include "ClockClient.h"
#include "Messages.h"
#include "Packer.h"


Network::ClockClient::ClockClient()
{
	m_heartbeat = 0;
}

Network::ClockClient::~ClockClient()
{

}

int Network::ClockClient::handler(int fd)
{
	printf("handeling fd %i\n", fd);
	
    return 0;
}
