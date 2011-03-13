#ifndef TEAM_H_
#define TEAM_H_

#include "Robot.h"
#include "Home.h"
#include <list>



namespace Game
{

class Team
{
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
     //TODO: Removed lists, may need later
private:

    /**
     * Home location.
     */
    Home* m_Home;
};
}

#endif
