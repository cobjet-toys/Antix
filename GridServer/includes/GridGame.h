#ifndef GAME_H_
#define GAME_H_

#include "Team.h"
#include "Puck.h"
#include <list>
#include <vector>
#include <map>
#include <stdint.h>

typedef std::vector<Robot*>::iterator RobotIter;
typedef std::vector<Puck*>::iterator PuckIter;


typedef struct{
    int robotid;
    float x;
    float y;
} sensed_item

typedef struct{
    int action;
    float speed;
    float angle;
} action

typedef struct{
    float x_pos;
    float y_pos;
    int speed;
    float angle;
    int puck_id;
} action_results

typedef struct{
    int teamid;
    float x_pos;
    float y_pos;
    float robot_x;
    float robot_y;
} team_start

class RobotGame
{
public:
    /**
     * All initializatoin of the game happens here.
     * Config options etc...
     */
    void init(int argc, char** argv);
    
    /**
     * Sorts the robot population
     */
    void sortRobots();

    /**
     * Dtor. Delete the pucks and teams.
     */
    ~RobotGame();

    /**
    * Receives a team id and team size and creates all robot with the ids
    * based on the team id. ie team id = 2, team_size= 100, result is robotid range 200-299
    */
    vector<team_start> randomizeTeam(int team_id, int team_size);

    /**
     * Interface function to Network layer for registering a robot.
     */
    int registerRobot(action robots);

    /**
     * Interface function to Network layer for unregistering a robot.
     */
    int unregisterRobot(action robot);

    /**
     * Interface function to Network layer for returning sensor data for a list of robots
     * for a set of robots on a client
     */
    int returnSensorData(vector<int>* robot_ids_from_client, map<int><vector<sensed_item>>* sensed_items_map);

    /**
     * Interface function to Network layer for processing actions for each robot.
     */
    int processAction(map<int><action>* robot_actions, map<int><action>* results);

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
    void addRobotToPop(Robot* robot);

    /**
     * Singleton instance that allows access anywhere Game.h is included.
     */
    static RobotGame* getInstance();

    /**
     * Redis client logging-wrapper instance.
     */
private:

    /**
     * Instance of our game.
     */
    static RobotGame* m_Instance;

    /**
     * List of teams.
     */
    std::vector<Team*> m_Teams;
    
    /**
     * List of pucks.
     */
    std::vector<Puck*> m_Pucks;

    /**
     * List of all available robots.
     */
    std::vector<Robot*> m_Population;

    //O(1) lookup of our sorted vector;     
    std::map<Robot*, int> m_XRobs;
    //Sorted list of robots; 
    std::vector<Robot*> m_XPos;

    //The max size of our world.
    float m_WorldSize;

    //Returns the max window size.
    unsigned int m_WindowSize;

    //Robot configurations.
    float robot_FOV = Math::dtor(90.0);
    float robot_Radius = 0.01;
    float robot_SensorRange = 0.1;
    float robot_PickupRange = robot_SensorRange/5.0);

    //Home radius.
    float home_Radius = 0.1;
    float WorldSize = 1.0;


};

#endif
