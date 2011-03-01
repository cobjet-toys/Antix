#ifndef __CLOCK_CLIENT_H__ 
#define __CLOCK_CLIENT_H__

#include "Client.h"

namespace Network
{

class ClockClient: public Client
{
public:
    ClockClient();
    virtual ~ClockClient();
    virtual int handler();
};
}

#endif
