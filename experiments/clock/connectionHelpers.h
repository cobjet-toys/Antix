#ifndef CONNECTIONHELPER_H
#define CONNECTIONHELPER_H
#include "TcpConnection.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct 
{
	TcpConnection con;
	addrinfo info;
} Connection;

namespace connHelper
{
	int createTcpServer(char * location, int port, TcpConnection * conn);
	
	int createTcpClient(char * location, int port, TcpConnection * conn);
}

#endif