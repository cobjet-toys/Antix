#ifndef TEAM_H_
#define TEAM_H_

#include "Game/Robot.h"
#include "Game/Home.h"
#include <list>

#include "GUI/Color.h"

//TODO: We only need color when graphics are enabled.

namespace Game
{

class Team
{
/**
 * Added the abstract of a team, which contains robots, a home, and a color.
 */
friend class Robotix;
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
     * Get the color for this team.
     */
    const GUI::Color* getColor()const;

    /**
     * Iterators for our collection of robots.
     */
    std::list<Robot*>::iterator getFirstRobot();
    std::list<Robot*>::iterator getLastRobot();
private:

    /**
     * Color of the team.
     */
    GUI::Color* m_Color;

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
