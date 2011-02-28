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
    virtual int handler() = 0;
    virtual int init(const char* port);
private:
    char m_Port[MAX_PORT_LENGTH];
    TcpConnection m_ServerConn;
    std::map<int, TcpConnection*> m_Clients;
};

}

#endif
