#include "Client.h"

Network::Client::Client()
{
	m_epfd = -1;
	m_serverList.clear();
}

int Network::Client::init()
{
	if ((m_epfd = epoll_create(10)) < 0)
	{
		if (DEBUG) printf("Could not create epoll");
		return -1;
	}	
	return 0;
}

int Network::Client::initConnection(const char* host, const char* port)
{
	if (strlen(host) > INET6_ADDRSTRLEN || strlen(port) > MAX_PORT_LENGTH )
	{
		if (DEBUG) perror("Invalid port or host length to init");
		return -2;
	}
	
	if (host == NULL || port == NULL)
	{
		if (DEBUG) perror("Invalid arguments to init");
	} 
	
	addrinfo l_hints, *l_result, *l_p;
	int l_resvalue = -1;
	
	memset(&l_hints, 0, sizeof(struct addrinfo) );
	
	l_hints.ai_family = AF_UNSPEC;
	l_hints.ai_socktype = SOCK_STREAM;
	
	if ((l_resvalue = getaddrinfo(host, port, &l_hints, &l_result)) == -1)
	{
		if (DEBUG) fprintf(stderr, "Could not get addrinfo: %s\n", gai_strerror(l_resvalue));
		return -1;
	}
	
	TcpConnection * conn = new TcpConnection();
	
	if (conn == NULL)
	{
		if (DEBUG) printf("Could not create new TcpConnection\n");
		return -1;
	}

	for (l_p = l_result; l_p != NULL; l_p = l_p->ai_next)
	{
		if ((l_resvalue =conn->socket(l_p)) != 0)
		{
			if (l_resvalue == -1) 
			{
				if (DEBUG) perror("Could not connect socket trying next\n");
			} else if (l_resvalue == -2)
			{
				if (DEBUG) perror("Argument error to socket\n");
			} else 
			{
				if (DEBUG) perror("What did I just return!?!?\n");
			}
			continue;
		}
		
		if ((l_resvalue = conn->connect(l_p)) != 0)
		{
			if (l_resvalue == -1)
			{
				if (DEBUG) perror("Could not connect to server/port\n");
			} else if (l_resvalue == -2)
			{
				if (DEBUG) perror("Argument error to connect\n");
			} else {
				if (DEBUG) perror("What did I just return!?!?\n");
			}
			conn->close();
			continue;
		}
		
		break;
	}
	
	if (l_p == NULL)
	{
		if (DEBUG) perror("No valid addrinfo");
		return -1;
	}
	
	int fileDesc = conn->getSocketFd();
	
	
	if ( setnonblock(fileDesc) == -1)
	{
		if (DEBUG) printf("Could not set socket to non-blocking\n");
	}
	
	if( addHandler(fileDesc, EPOLLET|EPOLLIN|EPOLLHUP, conn ) == -1)
	{
		if (DEBUG) printf("Could not add handler to tcpConnection");
		return -1;
	}
	
	return 0;

}


int Network::Client::addHandler(int fd, unsigned int events, TcpConnection * connection)
{
	m_serverList[fd] = connection;
	epoll_event* e = new epoll_event[1];
	e[0].data.fd = fd;
	e[0].events = events;
	if (handle_epoll(m_epfd, EPOLL_CTL_ADD, fd, e) != 0)
	{
		if (DEBUG) printf("\n");
		return -1;
	} 
    else
    {
        return 0;
    }
}

int Network::Client::handle_epoll(int epfd, int op, int fd, epoll_event* event)
{
    int ret = epoll_ctl(epfd, op, fd, event);
    if(ret == -1)
    {
        printf("error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int Network::Client::setnonblock(int fd)
{
  int flags;
  flags = fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;
  return fcntl(fd, F_SETFL, flags);  
}

int Network::Client::start()
{
    for (;;)
	{
      	epoll_event * e = new epoll_event[10];
		int nfd;		
		nfd = epoll_wait(m_epfd, e, 10, 0);
        //printf("nfd: %i\n", nfd);
        for (int i = 0; i < nfd; i++)
        {    
	    	if (e[i].events & EPOLLRDHUP || e[i].events & EPOLLHUP || e[i].events & EPOLLERR)
	    	{
	            printf("Client Hangup/Error \n");
	    		handle_epoll(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL); // @todo add error checking
	    	} 
           else if (e[i].events & EPOLLIN) 
           {
		    	printf("Handling \n");
		    	int ret = handler(e[i].data.fd);
		    	printf("Handler returned %i\n", ret);
		    	if (ret < 0)
			    {
			        if (handle_epoll(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL) != 0)
				    {
				    	return -1;
				    }
			    }
		   }
        }
        delete e;
    }
	return 0;
}

/*
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
	
	if ((m_epfd = epoll_create(10)) == -1)
	{
		perror("EPOLL_CREATE error");
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
	
	int fd = m_conn.getSocketFd();
	epoll_event e;
	e.data.fd = fd;
	e.events = EPOLLOUT|EPOLLHUP;
	
	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd,&e) == -1)
	{
		perror("EPOLL error epoll_ctl:ADD");
		return -1;
	}
	
	if (setnonblock(fd) == -1)
	{
		perror("Could not set socket to non-block");
		return -1;		
	}
	
	
	freeaddrinfo(l_result);
	return 0;
	
}

Network::Client::~Client()
{
	m_conn.close();
}

int Network::Client::setnonblock(int fd)
{
  int flags;
  flags = fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;
  return fcntl(fd, F_SETFL, flags);
}

int Network::Client::start()
{
	for (;;)
	{
		epoll_event * e = new epoll_event[10];
		int nfd;		
		nfd = epoll_wait(m_epfd, e, 10, 500);
        printf("nfd: %i\n", nfd);

		for (int i = 0; i < nfd ; i++)
		{
				if (e[i].events & EPOLLRDHUP || e[i].events & EPOLLHUP || e[i].events & EPOLLERR)
				{
				    printf("Client Hangup/Error \n");
					epoll_ctl(m_epfd, EPOLL_CTL_DEL, e[i].data.fd, NULL); // @todo add error checking
				} 
                else if (e[i].events & EPOLLOUT)
				{
					printf("Handling \n");
					int ret = handler(e[i].data.fd);
					printf("Handler returned %i\n", ret);
					if (ret < 0)
					{
					    if (epoll_ctl(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL) != 0)
						{
							return -1;
						}
					}
				}
		}
        delete e;
	}
	
	return 0;
}
*/
