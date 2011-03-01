#ifndef GRID_SERVER_H_
#define GRID_SERVER_H_

#include "Server.h"
#include "Messages.h"

namespace Network
{
class GridServer : public Server
{
public:
    GridServer();
    virtual int handler(int fd);
};
}

#endif
