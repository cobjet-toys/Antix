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

class GridGame
{
public:
    /**
     * All initializatoin of the game happens here.
     * Config options etc...
     */
     GridGame(int gridid, int num_of_teams, int robots_per_team, int id_from, int id_to);
    
    /**
     * Dtor. Delete the pucks and teams.
     */
    ~GridGame();

    /**
     * Sorts the population
     */
    void sortPopulation();

    /*
    * Returns a team and a vector of robots as requested by the client. 
    * Must keep some kind of track of which robots its getting so each time this is called
    * there is not one of the same team or robot sent.
    */
    int getRobots(int& teamid, float& team_x, float& team_y, std::vector<robot_info>* robots);

    /**
    * Receives a team id and team size and creates all robot with the ids
    * based on the team id. ie team id = 2, team_size= 100, result is robotid range 200-299
    */
    //int initializeTeam(std::vector<int> team_ids, std::vector<robot_info>* robot_info_vector);

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

    /**
     * Add a robot to the general population.
     * Used when a team is initialized.
     */
    int addObjectToPop(Game::GameObject* object);

    int removeObjectFromPop(Game::GameObject* object);

    int removeObjectFromPop(int objectid);

    void printPopulation();

private:

    /**
     * List of pucks.
     */
    //std::vector<Game::Puck*> m_Pucks;

    /**
     * List and map of all available robots.
     */
    std::vector<Game::GameObject*> m_Population;

    /**
     * List and map of all available robots.
     */
    //std::vector<Game::GameObject*> m_SortedObjects;

    std::vector<Game::Team*> m_Teams;

    //O(1) lookup of robots by id;     
    std::map<int, Game::GameObject*> m_MapPopulation;


    //O(1) lookup of our sorted vector;     
    std::map<Game::GameObject*, int> m_YObjects;

    // read an ID
    unsigned int readId(unsigned int id);

    // read an ID
    unsigned int writeId(unsigned int id, int type);

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

    int m_Num_Of_Teams;
    int m_Robots_Per_Team;

    int teamcounter;
    int robotcounter;


};

#endif
