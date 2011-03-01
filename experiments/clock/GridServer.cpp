#include "GridServer.h"

using namespace Network;

GridServer::GridServer():Server()
{
}

int GridServer::handler(int fd)
{
    printf("got a connection");
	
}


