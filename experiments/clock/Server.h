#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_

#include "TcpConnection.h"
#include <map>

namespace Network
{

typedef std::map<int, TcpConnection*> TcpMap;

class Server
{
public:
    Server();
    virtual ~Server();
    virtual int handler() = 0;
    virtual int init(const char* port);
private:
    char m_Port[MAX_PORT_LENGTH];
protected:
    TcpConnection m_ServerConn;
    TcpMap m_Clients;
};

}

#endif
