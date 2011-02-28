#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_

#include "TcpConnection.h"
#include <map>

namespace Network
{

class Server
{
public:
    Server(char* port);
    //virtual int handler();
    virtual int init();
private:
    char* m_Port;
    TcpConnection m_ServerConn;
    std::map<int, TcpConnection*> m_Clients;
};

}

#endif
