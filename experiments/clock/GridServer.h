#ifndef GRID_SERVER_H_
#define GRID_SERVER_H_

#include "Server.h"
#include "Messages.h"
#include "Packer.h"

namespace Network
{
class GridServer : public Server
{
public:
    GridServer();
    virtual int handler(int fd);
	
private:
	uint32_t m_uId;
    uint32_t m_hb_rcvd;
};
}

#endif