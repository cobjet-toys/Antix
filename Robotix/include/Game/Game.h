#ifndef GAME_H_
#define GAME_H_

#include "Game/Team.h"
#include "Game/Puck.h"
#include <list>
#include <stdint.h>

namespace Game
{

class Robotix 
{
public:
    /**
     * All initializatoin of the game happens here.
     * Config options etc...
     */
    void init();
    
    /**
     * Main game loop. All game logic goes here.
     */
    void update();

    /**
     * Sorts the robot population
     */
    void sortRobots();

    /**
     * Dtor. Delete the pucks and teams.
     */
    ~Robotix();

    /**
     * Return the gui window size.
     */
    const unsigned int& getWindowSize() const;

    /**
     * Return the max world size.
     */
    const float& getWorldSize() const;

    //Used for iterating over our collections.
    std::list<Puck*>::iterator getFirstPuck();
    std::list<Puck*>::iterator getLastPuck();

    std::list<Team*>::iterator getFirstTeam();
    std::list<Team*>::iterator getLastTeam();

    std::list<Robot*>::iterator getFirstRobot();
    std::list<Robot*>::iterator getLastRobot();

    /**
     * Add a robot to the general population.
     * Used when a team is initialized.
     */
    void addRobotToPop(Robot* robot);

    /**
     * Singleton instance that allows access anywhere Game.h is included.
     */
    static Robotix* getInstance();
private:

    /**
     * Instance of our game.
     */
    static Robotix* m_Instance;

    /**
     * List of teams.
     */
    std::list<Team*> m_Teams;
    
    /**
     * List of pucks.
     */
    std::list<Puck*> m_Pucks;

    /**
     * List of all available robots.
     */
    std::list<Robot*> m_Population;

    //Total number of teams.
    unsigned int m_TotalTeamCount;

    //Total number of pucks.
    unsigned int m_TotalPuckCount;

    //The max size of our world.
    float m_WorldSize;
    uint64_t m_MaxUpdates;
    uint64_t m_Updates;
    int m_SleepMsec;

    //Is the game paused.
    bool m_GamePaused;

    //TODO implement data.
    bool m_ShowData;

    //Returns the max window size.
    unsigned int m_WindowSize;
};

}
#endif
