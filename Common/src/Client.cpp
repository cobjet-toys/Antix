#include "Client.h"
#include "Config.h"

Network::Client::Client()
{
	m_epfd = -1;
	m_serverList.clear();
}

int Network::Client::init()
{
	if ((m_epfd = epoll_create(1000)) < 0)
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
		if (DEBUG) perror("Invalid port or host length to init\n");
		return -2;
	}
	
	if (host == NULL || port == NULL)
	{
		if (DEBUG) perror("Invalid arguments to init");
	} 
	
	DEBUGPRINT("Connection to host:%s:%s\n", host, port);
	
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
	
	if( addHandler(fileDesc, EPOLLET|EPOLLIN|EPOLLHUP|EPOLLPRI, conn ) == -1)
	{
		if (DEBUG) printf("Could not add handler to tcpConnection\n");
		return -1;
	}
	
	return fileDesc;

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
    epoll_event * e = new epoll_event[1000];
    for (;;)
	{
      	int nfd;		
		nfd = epoll_wait(m_epfd, e, 1000, 0);
        //printf("nfd: %i\n", nfd);
        for (int i = 0; i < nfd; i++)
        {    
            if (e[i].events & EPOLLIN)
	    	{
	            //DEBUGPRINT("Handling \n");
		    	int ret = handler(e[i].data.fd);
		    	//printf("Handler returned %i\n", ret);
		    	if (ret < 0)
			    {
			        if (handle_epoll(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL) != 0)
				    {
				    	return -1;
				    }
			    }
            } 
           else if (e[i].events & EPOLLRDHUP || e[i].events & EPOLLHUP || e[i].events & EPOLLERR) 
           {
		    	DEBUGPRINT("Client Hangup/Error \n");
	    		handle_epoll(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL); // @todo add error checking

		   }
        }
    }
    delete []e;

	return 0;
}
