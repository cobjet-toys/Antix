#ifndef GRID_SERVER_H_
#define GRID_SERVER_H_

#include "Server.h"
#include "Messages.h"
#include "Packer.h"
#include "GridGame.h"

namespace Network
{
class GridServer : public Server
{
public:
    GridServer();
    virtual int handler(int fd);
    virtual int handleNewConnection(int fd);
    virtual int allConnectionReadyHandler();

private:
    uint32_t m_uId;
    uint32_t m_hb_rcvd;
    GridGame* gridGameInstance;	
};
}

#endif
