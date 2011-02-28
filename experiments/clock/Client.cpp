#include "client.h"

Network::client::client()
{
	memset(&m_info, 0, sizeof(struct addrinfo));
	m_host = NULL;
	m_port = NULL;
}

int Network::client::init(char* host, char * port)
{
	if (host == NULL || port == NULL) return -2;
	
	struct addrinfo l_hints, *l_result, *l_p;
	int l_resvalue = 0;
	
	memset(&l_hints, 0, sizeof(struct addrinfo) );
	l_hints.ai_family = AF_UNSPEC;
	l_hints.ai_protocol = SOCK_STREAM;
	
	if ((l_resvalue = getaddrinfo(host, port, &l_hints, &l_result)) != 0)
	{
		fprintf(stderr, "Could not get addrinfo %s\n", gai_error(l_resvalue));
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
	
	if (p == NULL)
	{
		perror("No valid addrinfo");
		return -1;
	}
	
	freeaddrinfo();
	return 0;
	
}

Network::client::~client()
{

}