#ifndef TEAM_H_
#define TEAM_H_

#include "Robot.h"
#include "Home.h"
#include <list>

namespace Network
{
    class DrawServer;
}


namespace Game
{

class Team
{
/**
 * Added the abstract of a team, which contains robots, a home, and a color.
 */
friend class Robotix;
friend class Network::DrawServer;

public:
    /**
     * Ctor. Initialize with random color, and home at random position.
     */
    Team();

    /**
     * Delete the robots, home, and color.
     */
    ~Team();
    
    /**
     * Get the home for this team.
     */
    Home* getHome();

    /**
     * Iterators for our collection of robots.
     */
    std::list<Robot*>::iterator getFirstRobot();
    std::list<Robot*>::iterator getLastRobot();
private:

    /**
     * Home location.
     */
    Home* m_Home;

    /**
     * Population of robots on team.
     */
    std::list<Robot*> m_Robots;

    /**
     * Number of robots per team.
     */
    static unsigned int m_RobotPopCount;
};
}

#endif
