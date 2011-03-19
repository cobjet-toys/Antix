#ifndef ROBOT_CLIENT_H_ 
#define ROBOT_CLIENT_H_

#include "Client.h"
#include "Messages.h"
#include "Packer.h"
#include <vector>
#include <map>

namespace Network
{

class RobotClient: public Client
{
public:
    RobotClient();
    virtual int handler(int fd);
    int handleNewGrid(int id); 
    int initGrid(const char * host, const char * port, const int id);
    int initClock(const char * host, const char * port);
private:

    int packHeaderMessage(unsigned char* buffer, uint16_t sender, uint16_t message);
    int recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize);
    int sendWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize);
    int sendRobotRequests();

	std::vector<int> m_Grids;
    std::map<int, int> m_GridIds;
    uint16_t m_HeartBeat;
    int m_ClockFd;
    unsigned int m_ReadyGrids;
};
}

#endif

