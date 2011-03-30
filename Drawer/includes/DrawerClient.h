#ifndef _DRAWERCLIENT_H
#define	_DRAWERCLIENT_H

#include "Client.h"
#include "Robot.h"
#include "Puck.h"
#include "Team.h"
#include "Position.h"

#include "Messages.h"
#include "Packer.h"
#include "networkCommon.h"
#include "Config.h"
#include "AntixUtil.h"
#include "Types.h"

#include <map>
#include <vector>
#include <time.h>

class Team;
class Puck;

namespace Network
{

    typedef std::vector<Game::Puck*>::iterator PuckIter;
    typedef std::vector<Game::Robot*>::iterator RobotIter;
    typedef std::vector<Game::Team*>::iterator TeamIter;
    typedef std::vector<Game::Puck*> PuckMap;
    typedef std::vector<Game::Robot*> RobotMap;
    typedef std::vector<Game::Team*> TeamMap;


	class DrawServer: public Client
	{	
	public:                      

	    DrawServer();
	    virtual ~DrawServer();
        static DrawServer* getInstance();

        // Client/connection methods
        virtual int handler(int fd);
		int initGrid(const char * host, const char * port, int id);
		int sendGridConfig(int grid_fd);
        void initTeams();
        void updateObject(Msg_RobotInfo newInfo);        
        
        void updateViewRange(float tl_x, float tl_y, float br_x, float br_y);        
        
        int getWindowSize() { return this->m_windowSize; }
        float getWorldSize() { return this->m_worldSize; }
        float getHomeRadius() { return this->m_homeRadius; }
        bool getFOVEnabled() { return this->m_FOVEnabled; }
        float getFOVAngle() { return this->m_FOVAngle; }
        float getFOVRange() { return this->m_FOVRange; }

        std::vector<Game::Team*>::iterator getFirstTeam() { return this->m_teams.begin(); }
        std::vector<Game::Puck*>::iterator getFirstPuck() { return this->m_pucks.begin(); }
        std::vector<Game::Robot*>::iterator getFirstRobot() { return this->m_robots.begin(); }
        std::vector<Game::Team*>::iterator getLastTeam() { return this->m_teams.end(); }
        std::vector<Game::Puck*>::iterator getLastPuck() { return this->m_pucks.end(); }
        std::vector<Game::Robot*>::iterator getLastRobot() { return this->m_robots.end(); }
        size_t getTeamsCount() { return this->m_teams.size(); }
        size_t getPucksCount() { return this->m_pucks.size(); }
        size_t getRobotsCount() { return this->m_robots.size(); }
        size_t initPucks(int size);
        size_t initRobots(int size);
                
        int m_totalNumTeams;
        
        // below block of functions *SHOULD* only be called by DrawerParser
        void setWindowSize(int val);
        void setWorldSize(float val) { this->m_worldSize = val; }
        void setHomeRadius(float val) { this->m_homeRadius = val; }
        void setFOVEnabled(bool val) { this->m_FOVEnabled = val; }
        void setFOVAngle(float val) { this->m_FOVAngle = val; }
        void setFOVRange(float val) { this->m_FOVRange = val; }
        void setDrawerDataType(char val) { this->m_drawerDataType = val; }

	private:
        int packHeaderMessage(unsigned char* buffer, uint16_t sender, uint16_t message);
        int recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize);
        int sendWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize);
        
        static DrawServer* m_instance;
        
        int m_windowSize;
        float m_worldSize;
        float m_homeRadius;
        bool m_FOVEnabled;
        float m_FOVAngle;
        float m_FOVRange;
        char m_drawerDataType;

        std::vector<Game::Puck*> m_pucks;
        std::vector<Game::Robot*> m_robots;
        std::vector<Game::Team*> m_teams;
        
        uint32_t m_framestep;
        float m_viewTL_x;
        float m_viewTL_y;
        float m_viewBR_x;
        float m_viewBR_y;
    };
}

#endif	/* _DRAWERCLIENT_H */

