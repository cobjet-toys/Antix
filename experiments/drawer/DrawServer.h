#ifndef _DRAWSERVER_H
#define	_DRAWSERVER_H

#include "logger/logger.h"
#include "dutils.h"

namespace Game
{
    struct Puck
    {
        float posX;
        float posY;
    };

    struct Team
    {
        float posX;
        float posY;
        int colR;
        int colG;
        int colB;
    };

    struct Robot
    {
        float posX;
        float posY;
        float orientation;

        Team* team;
        bool hasPuck;
    };
    
    typedef std::vector<Puck*>::iterator PuckIter;
    typedef std::vector<Robot*>::iterator RobotIter;
    typedef std::vector<Team*>::iterator TeamIter;

}

namespace Network
{

    class DrawServer {

	public:
	    DrawServer();
	    virtual ~DrawServer();
            static DrawServer* getInstance();
            void init(int argc, char** argv);
	    void update();

            int getWindowSize() { return this->m_windowSize; }
            float getWorldSize() { return this->m_worldSize; }
            float getFOVAngle() { return this->m_FOVAngle; }
            float getFOVRange() { return this->m_FOVRange; }
            float getTeamRadius() { return this->m_teamRadius; }
            bool getFOVEnabled() { return this->m_FOVEnabled; }

            Game::TeamIter getFirstTeam() { return this->m_teams.begin(); }
            Game::PuckIter getFirstPuck() { return this->m_pucks.begin(); }
            Game::RobotIter getFirstRobot() { return this->m_robots.begin(); }
            Game::TeamIter getLastTeam() { return this->m_teams.end(); }
            Game::PuckIter getLastPuck() { return this->m_pucks.end(); }
            Game::RobotIter getLastRobot() { return this->m_robots.end(); }
            size_t getTeamsCount() { return this->m_teams.size(); }
            size_t getPucksCount() { return this->m_pucks.size(); }
            size_t getRobotsCount() { return this->m_robots.size(); }

	private:
            static DrawServer* m_instance;
            vector<Game::Puck*> m_pucks;
            vector<Game::Robot*> m_robots;
            vector<Game::Team*> m_teams;
            int m_windowSize;
            float m_worldSize;
            float m_FOVAngle;
            float m_FOVRange;
            float m_teamRadius;
            bool m_FOVEnabled;

            uint32_t m_timestep;
	    AntixUtils::Logger* m_posDB;
    };
}

#endif	/* _DRAWSERVER_H */

