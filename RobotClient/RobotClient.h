#ifndef ROBOT_CLIENT_H_ 
#define ROBOT_CLIENT_H_

#include "Client.h"
#include "Messages.h"
#include "Packer.h"

namespace Network
{

class RobotClient: public Client
{
public:
    RobotClient();
    virtual int handler(int fd);

private:
	uint16_t m_heartbeat;
};
}

#endif
