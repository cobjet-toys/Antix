#include "connectionHelpers.h"

Connection connHelper::createTcpServer(char * location, int port, TcpConnection * conn)
{
	TcpConnection server;

}

Connection connHelper::createTcpClient(char* location, int port, TcpConnection * conn)
{
	Connection con;
	
	TcpConnection client;
	
	struct addrinfo hints, *serverInfo, *p;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int errorno = 0;
	
	if ((errorno = getaddrinfo(location, port, &hints, &serverInfo)) == -1 )
	{
			return errorno;
	}
	
	for(p = serverInfo; p != NULL; p = p->ai_next)
	{
		if ((errorno = client->connect(p)) == -1)
		{
			continue;
		}
		
		break;
	}
	
	printf("have allocated resource");
}