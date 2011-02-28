#include "client.h"

Network::client::client()
{
	memset(&m_info, 0, sizeof(struct addrinfo));
	m_host = NULL;
	m_port = NULL;
}

int Network::client::init(char* host, char * port)
{
	struct addrinfo hints, *result, *p;
	
	memset(&hints, 0, sizeof(struct addrinfo) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_protocol = SOCK_STREAM;
	
	if (getaddrinfo(host, port, &hints, &result) == 0
	
}

Network::client::~client()
{

}