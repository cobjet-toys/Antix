#ifndef CLIENT_H
#define CLIENT_H

#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "TcpConnection.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>

namespace Network
{
	
class Client
{
public:
	Client();
	int init(char * host, char * port);
	virtual ~Client();
	virtual int handler() = 0;
		
private:
	
	char m_host[INET6_ADDRSTRLEN];
    char m_port[MAX_PORT_LENGTH];
    struct addrinfo m_info;
protected:
    TcpConnection m_conn;

};

}

#endif
