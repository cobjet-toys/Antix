#ifndef GRID_SERVER_H_
#define GRID_SERVER_H_

#include "Server.h"

namespace Network
{
class GridServer : public Server
{
public:
    GridServer();
    virtual int handler();
};
}

#endif
