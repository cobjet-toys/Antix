#include "Server.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "Config.h"

using namespace Network;


Server::Server()
{
    memset(&m_Port, 0, sizeof m_Port);
    m_Clients.clear();
    m_ServerConn = TcpConnection(); 
	m_epfd = -1;
	m_servers_total = 0;
	m_servers_connected = 0;
	m_ready = false;
}

int Server::init(const char* port, int maxConnections)
{
	m_servers_total = maxConnections;
	if (maxConnections == -1) m_ready = true;
	
    printf("Attempting to create a server socket on port: %s\n", port);
    m_epfd = epoll_create(1000);
	if (m_epfd < 0 ) return -1;
    //Copy the port to m_Port.
    size_t l_PortSize = strlen(port);
    if (l_PortSize > (MAX_PORT_LENGTH-1))
    {
        perror("Port is of invalid size\n");
        return -2;
    }
    strncpy(m_Port, port, l_PortSize);
    m_Port[l_PortSize] = '\0';

    struct addrinfo l_Hints, *l_ServerInfo; 

    memset(&l_Hints, 0 , sizeof l_Hints);    
    l_Hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6
    l_Hints.ai_socktype = SOCK_STREAM;
    l_Hints.ai_flags = AI_PASSIVE; // Use local IP
    
    int l_ResValue = 0;
    if ((l_ResValue = getaddrinfo(NULL, m_Port, &l_Hints, &l_ServerInfo)) != 0)
    {
        fprintf(stderr, "Error in getaddrinfo: %s\n", gai_strerror(l_ResValue));
        return -1; 
    }

    struct addrinfo *l_AddrIter; 
    for(l_AddrIter = l_ServerInfo; l_AddrIter != NULL; l_AddrIter = l_AddrIter->ai_next)
    {
        if ((l_ResValue = m_ServerConn.socket(l_AddrIter)) != 0)
        {
            if (l_ResValue == -1)
            {
                perror("Couldn't create socket from addrinfo.\n");
                continue;
            }
            else if (l_ResValue == -2)
            {
                perror("Invalid arguments passed to 'socket'\n");
                return -2;
            }
            else
			{
                perror("WTF is Hardeep returning.\n");
				return -3;
			}
        }

        //TODO Set SOCKET REUSEADDR option.

        if ((l_ResValue = m_ServerConn.bind(l_AddrIter)) != 0)
        {
            m_ServerConn.close();
            if (l_ResValue == -1)
            {
                perror("Could bind to socket from addrinfo.\n");
                continue;
            }
            else if (l_ResValue == -2)
            {
                perror("Invalid arguments passed to 'bind'.\n");
                return -2;
            }
            else
			{
                perror("WTF is Hardeep returning.\n");
				return -3;
			}
        }
       
       break; 
    }

    if (l_AddrIter == NULL)
    {
        fprintf(stderr, "Couldn't create a new socket.\n");
        return -1;
    }

    freeaddrinfo(l_ServerInfo);

   if ((l_ResValue = m_ServerConn.listen()) != 0)
    {
        if (l_ResValue == -1)
        {
            perror("Failed to listen on socket\n");
            return -1;
        }
        else if (l_ResValue == -2)
        {
            perror("Invalid argument in 'listen'.\n");
            return -2;
        } else {
			perror("WTF is Hardeep returning.\n");
			return -3;
		}
    }
	int fileDesc = m_ServerConn.getSocketFd();
	int enable = 1;
	
	if (setsockopt(fileDesc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
	{
		return -1;	
	}
	
	this->setnonblock(fileDesc); // @ todo check for errors
	this->addHandler(fileDesc, EPOLLIN|EPOLLET, &m_ServerConn); //@ todo check for errors
	
    printf("Server initialized and listening on port: %s\n", m_Port);
	
	return 0;
}


int Server::modifyHandler(int fd, unsigned int events)
{

	epoll_event e;
	e.data.fd = fd;
	e.events = events;
	if (handle_epoll(m_epfd, EPOLL_CTL_MOD, fd, &e) != 0)
	{
		DEBUGPRINT("Failed to add epoll handler\n");
		return -1;
	} 
    else
    {
        return 0;
    }
}

int Server::addHandler(int fd, unsigned int events, TcpConnection * connection)
{
	m_Clients[fd] = connection;
	epoll_event* e = new epoll_event[1];
	e[0].data.fd = fd;
	e[0].events = events;
	if (handle_epoll(m_epfd, EPOLL_CTL_ADD, fd, e) != 0)
	{
		return -1;
	} 
    else
    {
        return 0;
    }
}

int Server::start()
{
	epoll_event * e = new epoll_event[1000];
	if (e == NULL) return -1;

	for (;;)
	{
		
		int nfd, l_ret;
		nfd = epoll_wait(m_epfd, e, 1000, -1);
        //printf("nfd: %i %d\n", nfd, m_ready);

		for (int i = 0; i < nfd ; i++)
		{
			if (e[i].data.fd == m_ServerConn.getSocketFd()) // new connection
			{
				if (m_servers_connected < m_servers_total || m_servers_total == -1)
				{
					TcpConnection * temp = m_ServerConn.accept();
					if (temp != NULL)
					{
						int fd = temp->getSocketFd();
						m_Clients[fd] = temp;
						this->setnonblock(fd); // @ todo check for errors
						if( this->addHandler(fd, EPOLLIN|EPOLLET|EPOLLRDHUP, temp) != 0 )
						{
							printf("failed connecting to client.\n");
							// @ todo -- attempt recovery
							return -1;
						}
						else
						{
							printf("A client connected.\n");
							m_servers_connected += 1;
							if ((l_ret = this->handleNewConnection(fd)) < 0) return l_ret;
							
							if (m_servers_total != -1 && m_servers_connected == m_servers_total) 
							{
								m_ready = true;
								if ((l_ret = this->allConnectionReadyHandler()) < 0) return l_ret;
							}							
						}
					}
				} else {
					
				}
			}
			else
			{
				if (e[i].events & EPOLLRDHUP || e[i].events & EPOLLHUP || e[i].events & EPOLLERR)
				{
				    printf("Client Hangup/Error\n");
					handle_epoll(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL); // @todo add error checking
					continue;
				} 
                else if (e[i].events & EPOLLIN)
				{
					if (m_ready)
					{
						//printf("Handling \n");
						int ret = handler(e[i].data.fd);
						//printf("Handler returned %i\n", ret);
						if (ret < 0)
						{
							if (handle_epoll(m_epfd, EPOLL_CTL_DEL,e[i].data.fd, NULL) != 0)
							{
								return -1;
							}
						}
						
						if( modifyHandler(e[i].data.fd, EPOLLET|EPOLLIN|EPOLLHUP ) == -1)
						{
							DEBUGPRINT("Could not add handler to tcpConnection\n");
							return -1;
						}						
						
					} else {
						printf("SERVER ERROR:\t Handleing but not all clients connected\n");
					}
				}
			}
		}
	}
    delete [] e;
	return 0;
}

int Server::setnonblock(int fd)
{
  int flags;

  flags = fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;
  return fcntl(fd, F_SETFL, flags);
}

int Server::handle_epoll(int epfd, int op, int fd, epoll_event* event)
{
    int ret = epoll_ctl(epfd, op, fd, event);
    if(ret != 0)
    {
        printf("error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
