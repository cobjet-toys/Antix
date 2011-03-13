#ifndef GAME_H_
#define GAME_H_

#include "Team.h"
#include "Puck.h"
#include "Robot.h"
#include "MathAux.h"
#include <list>
#include <vector>
#include <map>
#include <stdint.h>

typedef std::vector<Game::Robot*>::iterator RobotIter;
typedef std::vector<Game::Puck*>::iterator PuckIter;


typedef struct{
    int robotid;
    float x;
    float y;
} sensed_item;

typedef struct{
    int action;
    float speed;
    float angle;
} action;

typedef struct{
    float x_pos;
    float y_pos;
    float speed;
    float angle;
    int puck_id;
} action_results;

typedef struct{
    unsigned int id;
    float x_pos;
    float y_pos;
    float speed;
    float angle;
    int puck_id;
} robot_info;


class GridGame
{
public:
    /**
     * All initializatoin of the game happens here.
     * Config options etc...
     */
     GridGame();
    
    /**
     * Sorts the robot population
     */
    void sortRobots();

    /**
     * Dtor. Delete the pucks and teams.
     */
    ~GridGame();

    /**
    * Receives a team id and team size and creates all robot with the ids
    * based on the team id. ie team id = 2, team_size= 100, result is robotid range 200-299
    */
    std::vector<robot_info> randomizeTeam(int team_id, int team_size);

    /**
     * Interface function to Network layer for registering a robot.
     */
    int registerRobot(robot_info robots);

    /**
     * Interface function to Network layer for unregistering a robot.
     */
    int unregisterRobot(int robot);

    /**
     * Interface function to Network layer for returning sensor data for a list of robots
     * for a set of robots on a client
     */
    int returnSensorData(std::vector<int>* robot_ids_from_client, std::map<int, std::vector<sensed_item> >* sensed_items_map);

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
     * List of all available robots.
     */
    std::vector<Game::Robot*> m_Population;

    //O(1) lookup of our sorted vector;     
    std::map<Game::Robot*, int> m_XRobs;
    //Sorted list of robots; 
    std::vector<Game::Robot*> m_XPos;

    //The max size of our world.
    float m_WorldSize;

    //Returns the max window size.
    unsigned int m_WindowSize;

    //Robot configurations.
    float robot_FOV;
    float robot_Radius;
    float robot_SensorRange;
    float robot_PickupRange;

    //Home radius.
    float home_Radius;
    float WorldSize;


};

#endif
