#ifndef GRIDGAME_H_
#define GRIDGAME_H_

#include "Team.h"
#include "Puck.h"
#include "Robot.h"
#include "MathAux.h"
#include "Types.h"
#include <list>
#include <vector>
#include <map>
#include <stdint.h>

typedef std::vector<Game::Robot*>::iterator RobotIter;
typedef std::vector<Game::Puck*>::iterator PuckIter;

class GridGame
{
public:
    /**
     * All initializatoin of the game happens here.
     * Config options etc...
     */
     GridGame();
    
    /**
     * Dtor. Delete the pucks and teams.
     */
    ~GridGame();

    /**
     * Sorts the robot population
     */
    void sortRobots();

    /**
    * Receives a team id and team size and creates all robot with the ids
    * based on the team id. ie team id = 2, team_size= 100, result is robotid range 200-299
    */
    int randomizeTeam(int team_id, int team_size, std::vector<robot_info>* robot_info_vector);

    /**
     * Interface function to Network layer for registering a robot.
     */
    int registerRobot(robot_info robot);

    /**
     * Interface function to Network layer for unregistering a robot.
     */
    int unregisterRobot(unsigned int robot);

    /*
     * Interface function to Network layer for returning sensor data for a list of robots
     * for a set of robots on a client
     */
    int returnSensorData(std::vector<int> robot_ids_from_client, std::map<int, std::vector<sensed_item> >* sensed_items_map);

    /**
     * Interface function to Network layer for processing actions for each robot.
     */
    int processAction(std::map<int, action>* robot_actions, std::map<int, action>* results);

    /**
     * Return the max world size.
     */
    const float& getWorldSize() const;

    //Used for iterating over our collections.
    PuckIter getFirstPuck();
    PuckIter getLastPuck();

    //Robot iterating
    RobotIter getFirstRobot();
    RobotIter getLastRobot();

    /**
     * Add a robot to the general population.
     * Used when a team is initialized.
     */
    void addRobotToPop(Game::Robot* robot);

    void printRobotPopulation();

private:

    /**
     * List of teams.
     */
    std::vector<Game::Team*> m_Teams;
    
    /**
     * List of pucks.
     */
    std::vector<Game::Puck*> m_Pucks;

    /**
     * List and map of all available robots.
     */
    std::vector<Game::Robot*> m_Population;


    //O(1) lookup of our sorted vector;     
    std::map<Game::Robot*, int> m_XRobs;
    //Sorted list of robots; 
    std::vector<Game::Robot*> m_XPos;

    //Returns the max window size.
    unsigned int m_WindowSize;

    //Robot configurations.
    float robot_FOV;
    float robot_Radius;
    float robot_SensorRange;
    float robot_PickupRange;

    //Home radius.
    float home_Radius;

    // Total world size
    float m_WorldSize;
    int m_NumGrids;
    int m_GridId;

    int m_PuckTotal;


};

#endif
