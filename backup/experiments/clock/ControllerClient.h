#ifndef CONTROLLER_CLIENT_H_
#define CONTROLLER_CLIENT_H_

#include "Client.h"

namespace Network
{
class ControllerClient : public Client
{
public:
    ControllerClient();
    virtual int handler();
private:
};
}

#endif
