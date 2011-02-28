#include "Server.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace Network;

Server::Server(char* port):m_Port(port), m_ServerConn()
{

}

int Server::init()
{
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
                perror("WTF is Hardeep returning.\n");
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
                perror("WTF is Hardeep returning.\n");
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
        }
    }

    printf("Server initialized and listening on port: %s\n", m_Port);
}
