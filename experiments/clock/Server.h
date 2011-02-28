#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_

#include "TcpConnection.h"
#include <map>

namespace Network
{

class Server
{
public:
    Server();
    //virtual int handler();
    virtual int init(const char* port);
private:
    char[MAX_PORT_LENGTH] m_Port;
    TcpConnection m_ServerConn;
    std::map<int, TcpConnection*> m_Clients;
};

}

#endif
