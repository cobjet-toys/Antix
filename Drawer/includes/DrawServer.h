#ifndef _DRAWSERVER_H
#define	_DRAWSERVER_H

#include "Server.h"
#include "Robot.h"
#include "Puck.h"
#include "Team.h"
#include "Position.h"

#include "Messages.h"
#include "Packer.h"
#include "networkCommon.h"
#include "Config.h"

#include <map>
#include <vector>

#define TEAM_ID_SHIFT       10

class Team;
class Puck;

namespace Network
{

    typedef std::map<int, Game::Puck*>::iterator PuckIter;
    typedef std::map<int, Game::Robot*>::iterator RobotIter;
    typedef std::map<int, Game::Team*>::iterator TeamIter;
    typedef std::map<int, Game::Puck*> PuckMap;
    typedef std::map<int, Game::Robot*> RobotMap;
    typedef std::map<int, Game::Team*> TeamMap;


    class DrawServer : public Server
    {
	public:                        
	
	    DrawServer();
	    virtual ~DrawServer();
        static DrawServer* getInstance();
        int init(int argc, char** argv);
        void initTeams();
        void updateObject(Msg_RobotInfo newInfo);

        // inherited from Server
        virtual int handler(int fd);
		virtual int handleNewConnection(int fd);
		virtual int allConnectionReadyHandler();

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
        int m_windowSize;
        float m_worldSize;
        float m_FOVAngle;
        float m_FOVRange;
        float m_homeRadius;
        bool m_FOVEnabled;

        uint32_t m_framestep;
    };
}

#endif	/* _DRAWSERVER_H */

