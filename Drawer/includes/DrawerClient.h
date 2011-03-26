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
class DrawParser;

namespace Network
{

    typedef std::map<int, Game::Puck*>::iterator PuckIter;
    typedef std::map<int, Game::Robot*>::iterator RobotIter;
    typedef std::map<int, Game::Team*>::iterator TeamIter;
    typedef std::map<int, Game::Puck*> PuckMap;
    typedef std::map<int, Game::Robot*> RobotMap;
    typedef std::map<int, Game::Team*> TeamMap;


	class DrawServer: public Client
	{
	friend class DrawParser;
	
	public:                      

	    DrawServer();
	    virtual ~DrawServer();
        static DrawServer* getInstance();

        // Client/connection methods
        virtual int handler(int fd);
		int initGrid(const char * host, const char * port, int id);
		int setGridConfig(int grid_fd, char send_data, float topleft_x = 0.0, float topleft_y = 0.0, float bottomright_x = 0.0, float bottomright_y = 0.0);
        void initTeams();
        void updateObject(Msg_RobotInfo newInfo);
        
        int m_windowSize;
        float m_worldSize;
        float m_FOVAngle;
        float m_FOVRange;
        float m_homeRadius;
        bool m_FOVEnabled;
        char m_drawerDataType;
        
        int getWindowSize() { return this->m_windowSize; }
        float getWorldSize() { return this->m_worldSize; }
        float getFOVAngle() { return this->m_FOVAngle; }
        float getFOVRange() { return this->m_FOVRange; }
        float getHomeRadius() { return this->m_homeRadius; }
        bool getFOVEnabled() { return this->m_FOVEnabled; }

        std::map<int, Game::Team*>::iterator getFirstTeam() { return this->m_teams.begin(); }
        std::map<int, Game::Puck*>::iterator getFirstPuck() { return this->m_pucks.begin(); }
        std::map<int, Game::Robot*>::iterator getFirstRobot() { return this->m_robots.begin(); }
        std::map<int, Game::Team*>::iterator getLastTeam() { return this->m_teams.end(); }
        std::map<int, Game::Puck*>::iterator getLastPuck() { return this->m_pucks.end(); }
        std::map<int, Game::Robot*>::iterator getLastRobot() { return this->m_robots.end(); }
        size_t getTeamsCount() { return this->m_teams.size(); }
        size_t getPucksCount() { return this->m_pucks.size(); }
        size_t getRobotsCount() { return this->m_robots.size(); }

	private:
        int packHeaderMessage(unsigned char* buffer, uint16_t sender, uint16_t message);
        int recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize);
        int sendWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize);
        
        static DrawServer* m_instance;

        std::map<int, Game::Puck*> m_pucks;
        std::map<int, Game::Robot*> m_robots;
        std::map<int, Game::Team*> m_teams;
        

        uint32_t m_framestep;
    };
}

#endif	/* _DRAWERCLIENT_H */

