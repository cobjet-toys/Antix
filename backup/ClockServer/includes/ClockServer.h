#ifndef __CLOCK_SERVER_H__ 
#define __CLOCK_SERVER_H__

#include "Server.h"
#include <map>

namespace Network
{
	
typedef std::map<int, TcpConnection*> TcpMap;

class ClockServer: public Server
{
public:
    ClockServer();
    virtual ~ClockServer();
    virtual int handler(int fd);

private:
	uint16_t m_heartbeat;
};
}

#endif
