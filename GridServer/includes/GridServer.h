#ifndef GRID_SERVER_H_
#define GRID_SERVER_H_

#include "Server.h"
#include "Messages.h"
#include "Packer.h"
#include "GridGame.h"

#include "AntixUtil.h"

#define FRAME_FREQUENCY 50000	//used as microseconds = 50 ms
#define NUM_NEIGHBOURS 2

namespace Network
{
class GridServer : public Server
{
public:
    GridServer();
	int initGridGame();
    virtual int handler(int fd);
    virtual int handleNewConnection(int fd);
	virtual int allConnectionReadyHandler();
    int initClock(const char* host, const char* ip);
	void setTeams(int amount);
	void setIdRange(int from, int to);
	void setRobotsPerTeam(int amount);
	void setId(int id);
	void setNumClients(int numClients);
		
    //Drawer Connections
    int updateDrawer(uint32_t framestep);

    GridGame* Game();
    int sendHeartBeat();
    void setSortDone();

private:
    bool m_Sorted;
    pthread_t m_SortThread;

    uint32_t m_uId;
    uint32_t m_hb_rcvd;
	uint32_t m_idRangeFrom;
	uint32_t m_idRangeTo;

	int m_numClients;

	float m_homeRadius; // information for the GridGame/GridClient
    float m_worldSize;
	int m_numGrids;
    int m_puckTotal;

	int m_robotsPerTeam;
	int m_teamsAvailable;
	int m_robotsAvailable;
	int m_robotsConfirmed;
 	int m_teamsConfirmed;
	int m_ControllerFd;
	//GridGame gridGameInstance;
    GridGame* gridGameInstance;	

    std::map<int, int> m_GridPosToFd;

    int m_ClockFd;
    uint16_t m_Hb;
    int m_ReadyPartners;

	bool m_updateDrawerFlag;
	float m_drawerTop;			//top edge in view
	float m_drawerBottom;		//bottom edge in view
	float m_drawerLeft;			//left edge in view
	float m_drawerRight;		//right edge in view
    TcpConnection * m_drawerConn;
};
}

#endif
