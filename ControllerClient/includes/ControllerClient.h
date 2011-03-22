#ifndef CONTROLLER_CLIENT_H_ 
#define CONTROLLER_CLIENT_H_

#include "Client.h"
#include "Messages.h"
#include "Packer.h"
#include <vector>
#include <map>
#include <utility> 

namespace Network
{

class ControllerClient: public Client
{
public:
    ControllerClient();
    virtual int handler(int fd);
    int initGrid(const char * host, const char * port);
    int initClock(const char * host, const char * port);
    int initRobotClient(const char* host, const char* port);

    //Initialize grids.
    int initNeighbourGrids(); 

    int beginSimulation();

private:

    std::map<int, std::pair<const char*, const char*> > m_GridConInfo;
    std::vector<int> m_RobotClients;
	std::vector<int> m_Grids;
    int m_ClockFd;

	uint32_t m_totalGrids;
	uint32_t m_totalRobotClients;
	
	uint32_t m_totalGridsPending;
	uint32_t m_totalGridsReady;
	uint32_t m_totalRobotClientsReady;
	
	uint32_t m_curRange;
};
}

#endif

