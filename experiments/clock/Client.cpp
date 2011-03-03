#include "Client.h"

Network::Client::Client()
{
	memset(&m_info, 0, sizeof(struct addrinfo));
	memset(&m_host, 0, INET6_ADDRSTRLEN);
	memset(&m_port, 0, MAX_PORT_LENGTH);
}

int Network::Client::init(const char* host, const char * port)
{
	if (strlen(host) > INET6_ADDRSTRLEN || strlen(port) > MAX_PORT_LENGTH )
	{
		perror("Invalid port or host length to init");
		return -2;
	}
	
	if (host == NULL || port == NULL) 
	{
		perror("Invalid arguments to init");
		return -2;
	}
	
	size_t l_hostlen= strlen(host); 
	size_t l_portlen= strlen(port); 
	
	strncpy(m_host, host, l_hostlen);
	m_host[l_hostlen] = '\0';
	
	strncpy(m_port, port, l_portlen);
	m_port[l_hostlen] = '\0';
	
	struct addrinfo l_hints, *l_result, *l_p;
	int l_resvalue = 0;
	
	memset(&l_hints, 0, sizeof(struct addrinfo) );
	l_hints.ai_family = AF_UNSPEC;
	l_hints.ai_socktype = SOCK_STREAM;
	
	if ((l_resvalue = getaddrinfo(m_host, m_port, &l_hints, &l_result)) != 0)
	{
		fprintf(stderr, "Could not get addrinfo %s\n", gai_strerror(l_resvalue));
		return -1;
	}
	
	for (l_p = l_result; l_p != NULL; l_p = l_p->ai_next)
	{
		if ((l_resvalue =m_conn.socket(l_p)) != 0)
		{
			if (l_resvalue == -1) 
			{
				perror("Could not bind socket trying next");
			} else if (l_resvalue == -2)
			{
				perror("Argument error to socket");
			} else 
			{
				perror("What did I just return!?!?");
			}
			continue;
		}
		
		if ((l_resvalue = m_conn.connect(l_p)) != 0)
		{
			if (l_resvalue == -1)
			{
				perror("Could not connect to server/port");
			} else if (l_resvalue == -2)
			{
				perror("Argument error to connect");
			} else {
				perror("What did I just return!?!?");
			}
			m_conn.close();
			continue;
		}
		
		break;
	}
	
	if (l_p == NULL)
	{
		perror("No valid addrinfo");
		return -1;
	}
	
	freeaddrinfo(l_result);
	return 0;
	
}

Network::Client::~Client()
{
	m_conn.close();
}
