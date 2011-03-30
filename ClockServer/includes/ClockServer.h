#ifndef __CLOCK_SERVER_H__
#define __CLOCK_SERVER_H__

#include "Server.h"
#include <map>
#include <vector>

namespace Network
{

typedef std::map<int, bool> clientMap;
typedef std::vector<int> clientList;

class ClockServer: public Server
{
public:
    ClockServer();
    virtual ~ClockServer();
    virtual int handler(int fd);
    virtual int handleNewConnection(int fd);
    virtual int allConnectionReadyHandler();
    int SendHeartBeat();

private:
    uint16_t m_heartbeat;
    clientMap m_clientMap;
    clientList m_clientList;
    uint16_t m_beat;
    int m_responded;
    TcpConnection * m_clockConn;
    bool m_ready;
    uint32_t m_timesteps;
};
}

#endif
