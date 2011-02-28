#include "GridServer.h"

using namespace Network;

GridServer::GridServer()
{
}

int GridServer::handler()
{
    struct sockaddr_storage l_ClienAddr;
    while(1)
    {
        TcpConnection *l_NewConn = m_ServerConn.accept();
        if (l_NewConn == NULL)
        {
            perror("Error accepting connection\n");
            continue;
        }
        printf("Received new connection\n");

        m_Clients[l_NewConn->getSocketFd()] = l_NewConn; 
    }
}


