#include "client.h"

Network::client::client()
{
	memset(m_info, 0, sizeof(struct addrinfo));
	m_host = NULL;
	m_port = NULL;
}

Network::client::client(char* host, char * port)
{
	struct addrinfo hints, *result, *p;
}

Network::client::~client()
{

}