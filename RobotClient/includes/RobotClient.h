#ifndef ROBOT_CLIENT_H_ 
#define ROBOT_CLIENT_H_

#include "Client.h"
#include "Messages.h"
#include "Packer.h"
#include <vector>
#include <map>
#include "RobotGame.h"

namespace Network
{

class RobotClient: public Client
{
public:
    RobotClient();
    ~RobotClient();
    virtual int handler(int fd);
    int handleNewGrid(int id); 
    int initGrid(const char * host, const char * port, const int id);
    int initClock(const char * host, const char * port);
private:

    int packHeaderMessage(unsigned char* buffer, uint16_t sender, uint16_t message);
    int sendRobotRequests();

	std::vector<int> m_Grids;
    std::map<int, int> m_GridIdToFd;
    std::map<int, int> m_GridFdToId;
    uint16_t m_HeartBeat;
    int m_ClockFd;
    unsigned int m_ReadyGrids;
    unsigned int m_ReadyActionGrids;
    RobotGame* robotGameInstance;

	uint16_t m_totalGridRequests;
	uint16_t m_totalGridResponses;
	uint16_t m_totalRobotsReceived;
};
}

#endif

