#ifndef GRID_SERVER_H_
#define GRID_SERVER_H_

#include "Server.h"
#include "Messages.h"
#include "Packer.h"
#include "GridGame.h"

#include "AntixUtil.h"

#define FRAME_FREQUENCY 50000	//used as microseconds = 50 ms

namespace Network
{
class GridServer : public Server
{
public:
    GridServer();
    virtual int handler(int fd);
    virtual int handleNewConnection(int fd);
	virtual int allConnectionReadyHandler();
	void setTeams(int amount);
	void setIdRange(int from, int to);
	void setRobotsPerTeam(int amount);
	void setId(int id);
		
    //Drawer Connections
    int updateDrawer(uint32_t framestep);

private:
    uint32_t m_uId;
    uint32_t m_hb_rcvd;
	int m_teamsAvailable;
	int m_idRangeFrom;
	int m_idRangeTo;
	int m_robotsPerTeam;
	//GridGame gridGameInstance;
    GridGame* gridGameInstance;	
	
	bool updateDrawerFlag;
    TcpConnection * m_drawerConn;
};
}

#endif
