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
		DEBUGPRINT("CLIENT ERROR:\t Could not create epoll");
		return -1;
	}	
	return 0;
}

int Network::Client::initConnection(const char* host, const char* port)
{
	if (strlen(host) > INET6_ADDRSTRLEN || strlen(port) > MAX_PORT_LENGTH )
	{
		DEBUGPRINT("CLIENT ERROR:\t Invalid port or host length to init\n");
		return -2;
	}
	
	if (host == NULL || port == NULL)
	{
		DEBUGPRINT("CLIENT ERROR:\t Invalid arguments to init\n");
	} 
	
	DEBUGPRINT("CLIENT STATUS:\t Connection to host:%s:%s\n", host, port);
	
	addrinfo l_hints, *l_result, *l_p;
	int l_resvalue = -1;
	
	memset(&l_hints, 0, sizeof(struct addrinfo) );
	
	l_hints.ai_family = AF_UNSPEC;
	l_hints.ai_socktype = SOCK_STREAM;
	
	if ((l_resvalue = getaddrinfo(host, port, &l_hints, &l_result)) == -1)
	{
		DEBUGPRINT("CLIENT ERROR:\t Could not get addrinfo: %s\n", gai_strerror(l_resvalue));
		return -1;
	}
	
	TcpConnection * conn = new TcpConnection();
	
	if (conn == NULL)
	{
		DEBUGPRINT("CLIENT ERROR:\t Could not create new TcpConnection\n");
		return -1;
	}

	for (l_p = l_result; l_p != NULL; l_p = l_p->ai_next)
	{
		if ((l_resvalue =conn->socket(l_p)) != 0)
		{
			if (l_resvalue == -1) 
			{
				DEBUGPRINT("CLIENT ERROR:\t Could not connect socket trying next\n");
			} else if (l_resvalue == -2)
			{
				DEBUGPRINT("CLIENT ERROR:\t Argument error to socket\n");
			} else 
			{
				DEBUGPRINT("CLIENT ERROR:\t Invalid return\n");
			}
			continue;
		}
		
		if ((l_resvalue = conn->connect(l_p)) != 0)
		{
			if (l_resvalue == -1)
			{
				DEBUGPRINT("CLIENT ERROR:\t Could not connect to %s:%s\n", host, port);
			} else if (l_resvalue == -2)
			{
				DEBUGPRINT("CLIENT ERROR:\t Argument error to connect\n");
			} else {
				DEBUGPRINT("CLIENT ERROR:\t Invalid return\n");
			}
			conn->close();
			continue;
		}
		
		break;
	}
	
	if (l_p == NULL)
	{
		DEBUGPRINT("CLIENT ERROR:\t Could not connect to %s:%s\n", host, port);
		return -1;
	}
	
	int fileDesc = conn->getSocketFd();
	int enable = 1;
	
	if (setsockopt(fileDesc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
	{
		return -1;	
	}
	
	if ( setnonblock(fileDesc) == -1)
	{
		DEBUGPRINT("CLIENT ERROR:\t Could not set socket to non-blocking\n");
	}
	
	if( addHandler(fileDesc, EPOLLET|EPOLLIN|EPOLLHUP, conn ) == -1)
	{
		DEBUGPRINT("CLIENT ERROR:\t Could not add handler to tcpConnection\n");
		return -1;
	}
	
	return fileDesc;

}


int Network::Client::addHandler(int fd, unsigned int events, TcpConnection * connection)
{
	m_serverList[fd] = connection;
	epoll_event e;
	e.data.fd = fd;
	e.events = events;
	if (handle_epoll(m_epfd, EPOLL_CTL_ADD, fd, &e) != 0)
	{
		DEBUGPRINT("CLIENT ERROR:\t Failed to add epoll handler\n");
		return -1;
	} 
    else
    {
        return 0;
    }
}

int Network::Client::modifyHandler(int fd, unsigned int events)
{

	epoll_event e;
	e.data.fd = fd;
	e.events = events;
	if (handle_epoll(m_epfd, EPOLL_CTL_MOD, fd, &e) != 0)
	{
		DEBUGPRINT("CLIENT ERROR:\t Failed to add epoll handler\n");
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
        DEBUGPRINT("CLIENT ERROR:\t %s\n", strerror(errno));
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
		nfd = epoll_wait(m_epfd, e, 1, 500);
        printf("nfd: %i\n", nfd);
        for (int i = 0; i < nfd; i++)
        {    
            if (e[i].events & EPOLLIN)
	    	{
	            //DEBUGPRINT("Handling \n");
		    	int ret = handler(e[i].data.fd);
		    	
		    	if (ret < 0)
			    {
			        if (handle_epoll(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL) != 0)
				    {
				    	return -1;
				    }
			    }
			    
			    if( modifyHandler(e[i].data.fd, EPOLLET|EPOLLIN|EPOLLHUP ) == -1)
				{
					DEBUGPRINT("CLIENT ERROR:\t Could not add handler to tcpConnection\n");
					return -1;
				}
            } 
           else if (e[i].events & EPOLLRDHUP || e[i].events & EPOLLHUP || e[i].events & EPOLLERR) 
           {
				DEBUGPRINT("CLIENT WARNING:\t Server Hangup/Error \n");
	    		handle_epoll(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL); // @todo add error checking

		   }
        }
    }
    delete []e;

	return 0;
}
