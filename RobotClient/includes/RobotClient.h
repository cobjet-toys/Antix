#ifndef ROBOT_CLIENT_H_ 
#define ROBOT_CLIENT_H_

#include "Client.h"
#include "Messages.h"
#include "Packer.h"
#include <vector>

namespace Network
{

class RobotClient: public Client
{
public:
    RobotClient();
    virtual int handler(int fd);
    int processRobots();
    int initGrid(const char * host, const char * port);

private:
	std::vector<int> m_Grids;
};
}

#endif
