#include "GridGame.h"
#include "Robot.h"
#include "Team.h"
#include "Position.h"
#include "GameObject.h"
#include "MathAux.h"
#include "Config.h"


#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <math.h>
using namespace Game;

#define MILLISECS_IN_SECOND 1000
#define PUCKVALUE 1000000000000000000000000000000
#define ROBOTVALUE 0000000000000000000000000000000
#define ROBOT 0
#define PUCK 1

typedef std::pair<int, GameObject*> ObjectIDPair;
typedef std::pair<int, std::vector<sensed_item> > ObjectPair;

GridGame::GridGame(int gridid, int num_of_teams, int robots_per_team, int id_from, int id_to)
{

    m_GridId = gridid;
    m_Num_Of_Teams = num_of_teams;
    m_Robots_Per_Team = robots_per_team;

    //Robot configurations.
    robot_FOV = Math::dtor(90.0);
    robot_Radius = 0.01;
    robot_SensorRange = 1;
    robot_PickupRange = robot_SensorRange/5.0;

    printf("Robot FOV: \n%f\n", robot_FOV);


    home_Radius = 0.1;
    m_WorldSize = 10;
	m_NumGrids = 2;
    m_PuckTotal = 20;

    // Safety
    m_Population.clear();
    m_Teams.clear();

    int l_pucks_per_grid = m_PuckTotal/m_NumGrids;
    int high_puck_range = l_pucks_per_grid * m_GridId;
    int low_puck_range = high_puck_range - l_pucks_per_grid + 1;

    //int high_puck_range = (m_PuckTotal/m_NumGrids)*(m_GridId+1);
    //int low_puck_range = (m_PuckTotal/m_NumGrids)*(m_GridId);

    Team* l_team;

    for (int i = id_from; i <= id_to; i++)
    {

        #ifdef DEBUG
        std::cout << "Robot id being created:" <<i << std::endl;
       // if you can divide the id by the robots per team, that means that this is a team, create a new team
       // object for the rest of the robots to point to

        std::cout << "WTF id:" <<i << "robots per team: "<< robots_per_team << std::endl;
        #endif
        if (i%robots_per_team == 0)
        {
            std::cout << "WTF2 id:" <<i << "robots per team: "<< robots_per_team << std::endl;
            Math::Position* l_RobotPosition = Math::Position::randomPosition(m_WorldSize, m_NumGrids, m_GridId);
            l_team = new Team(l_RobotPosition, i/robots_per_team);
            m_Teams.push_back(l_team);
        }

        Math::Position* l_RobotPosition = Math::Position::randomPosition(m_WorldSize, m_NumGrids, m_GridId);
        Game::Robot* l_Robot = new Robot(l_RobotPosition, i );
        addObjectToPop(l_Robot);
    }

    // TEMPORARY until I write the bit shifting functions
    high_puck_range = high_puck_range + 1000000;
    low_puck_range = low_puck_range + 1000000;

    printf("high:%d - low:%d", high_puck_range, low_puck_range);


    //Generate pucks at random locations.
    for (uint i = low_puck_range; i <= high_puck_range; i++)
    {
       std::cout << i << std::endl;
       //Get a random location.
       Math::Position* l_PuckPos = Math::Position::randomPosition(m_WorldSize, m_NumGrids, m_GridId);

       //Create the puck.
       Puck* l_Puck = new Puck(l_PuckPos, i);
       addObjectToPop(l_Puck);
    }

    //exit(1);
    
    //TODO: Wrap this in ifdef debug
    for(std::vector<Team*>::iterator it = m_Teams.begin(); it != m_Teams.end(); it++)
    {
    
        std::cout << "Id:" << (*it)->getId() << "Team X: " << (*it)->getX() << "Team Y:" << (*it)->getY() << std::endl;

    }


    // Sort generated pucks
    sortPopulation();

}

GridGame::~GridGame()
{
    //Note: The delete of the robot population is the responsiblity of the team object.
   
    //Delete the Robots.
    for(std::vector<GameObject*>::iterator it = m_Population.begin(); it != m_Population.end(); it++)
    {
        delete (*it);
    }

}

void GridGame::sortPopulation()
{
    //Sort on the x axis.
    int i;
    GameObject* l_Key;
    for(unsigned int j = 1; j < m_Population.size(); j++)
    {
        l_Key = m_Population[j];
        i = j - 1;
        while(i >= 0 && m_Population[i]->getPosition()->getY() > l_Key->getPosition()->getY())
        {
            m_Population[i+1] =m_Population[i];
            m_YObjects[m_Population[i+1]] = i+1;
            --i;
        }

        m_Population[i+1] = l_Key;
        m_YObjects[l_Key] = i+1;
    }
}


/*
int GridGame::initializeTeam(std::vector<int> teams, std::vector<robot_info>* robot_info_vector)
{

        //TODO: Will load team_size from the config
        int team_size = 1000;
        std::vector<int>::iterator end = teams.end();

        std::vector<robot_info> l_robot_info_vector;

        for(std::vector<int>::iterator it = teams.begin(); it != end; it++){
            // get range of robot ids based on team id and team size
            int firstid = (*it)*team_size;
            int lastid = firstid + team_size - 1;

            std::cout << "from: " << firstid << "to: " << lastid << std::endl;

            for (int i = firstid; i <= lastid; i++)
            {
                Math::Position* l_RobotPosition = Math::Position::randomPosition(m_WorldSize, m_NumGrids, m_GridId);
                Game::Robot* l_Robot = new Robot(l_RobotPosition, i, robot_FOV, robot_Radius, robot_PickupRange, robot_SensorRange);
                addObjectToPop(l_Robot);

                robot_info temp;
                temp.id = i;
                temp.x_pos = l_RobotPosition->getX();
                temp.y_pos = l_RobotPosition->getY();

                l_robot_info_vector.push_back(temp);
            }
        }
        robot_info_vector = &l_robot_info_vector;

        return 1;
}

*/

int GridGame::registerRobot(robot_info robot)
{
    // create a new position object for the new robot
    Math::Position* l_RobotPosition = new Position(robot.x_pos, robot.y_pos, robot.angle);

    // create new robot based on info from robot_info struct
    Game::Robot* l_Robot = new Robot(l_RobotPosition, robot.id);
			
    // add robot to the population
    addObjectToPop(l_Robot);

    //TODO: Sort robots at this point?

    return 1;
    
}

int GridGame::unregisterRobot(unsigned int robotid)
{

    int removeResult = removeObjectFromPop(robotid);    


    return removeResult;



}

int GridGame::returnSensorData(std::vector<int> robot_ids_from_client, std::map<int, std::vector<sensed_item> >* sensed_items_map)
{

    for( std::map<GameObject*, int>::iterator it = m_YObjects.begin(); it != m_YObjects.end(); it++){

        std::cout << it->first << " => " << it->second << std::endl;

    }

    std::map<int, std::vector<sensed_item> > l_sensed_items_map; 

    std::vector<int>::iterator clientend = robot_ids_from_client.end();
    for(std::vector<int>::iterator it = robot_ids_from_client.begin(); it != clientend; it++)
    {
        // grab GameObject from int id
        GameObject* tempobj = m_MapPopulation[(*it)]; 

        // grab vector position in sorted population
        int position = m_YObjects[tempobj];

        printf("position for gameobject id %d: %d\n", (*tempobj).m_id, position); 

        GameObject* position_obj = m_Population[position];

        
        std::vector<sensed_item> temp_vector;
        
        int counter = position;

        while ( counter >= 0 )
        {
            if ( (tempobj->getY() - position_obj->getY()) < robot_SensorRange ){
                //TODO: Check Y bound as well

                if ( abs(tempobj->getX() - position_obj->getX()) < robot_SensorRange ){
                    sensed_item temp_sensed;
                    temp_sensed.id = (*position_obj).m_id;
                    temp_sensed.x = position_obj->getX();
                    temp_sensed.y = position_obj->getX();
                    
                    temp_vector.push_back( temp_sensed );
                }
                counter--;
                position_obj = m_Population[counter];
            }
            else{
                break;
            }
        }

        counter = position;
        position_obj = m_Population[position];

        printf("Counter: %d - Size client list: %u\n", counter, robot_ids_from_client.size() );

        while ( counter <= m_Population.size() )
        {
            if ( (position_obj->getY() - tempobj->getY()) < robot_SensorRange ){
                //TODO: Check Y bound as well

                if ( abs(position_obj->getX() - tempobj->getX()) < robot_SensorRange ){
                    sensed_item temp_sensed;
                    temp_sensed.id = (*position_obj).m_id;
                    temp_sensed.x = position_obj->getX();
                    temp_sensed.y = position_obj->getX();
                    
                    temp_vector.push_back( temp_sensed );
                }
                counter++;
                position_obj = m_Population[counter];
            }
            else{
                break;
            }
        }

        l_sensed_items_map[(*it)] = temp_vector;
       
        #ifdef DEBUG
        int total = 0;

        for( std::vector<sensed_item>::iterator someit = temp_vector.begin(); someit != temp_vector.end(); someit++ ){
            std::cout << (*someit).id << ",";
            total++;
        }
        std::cout << "Total:" <<total << std::endl;
        #endif

    }

    sensed_items_map = &l_sensed_items_map;

    #ifdef DEBUG
    for( std::map<int, std::vector<sensed_item> >::iterator it = l_sensed_items_map.begin(); it != l_sensed_items_map.end(); it++){

        std::cout << "robot id:" << it->first << " => "; 
        
        for( std::vector<sensed_item>::iterator iit = (it->second).begin(); iit != (it->second).end(); iit++){
            std::cout << (*iit).id << ",";
        }
        std::cout << std::endl;

    }
    #endif

    return 1;

}



int GridGame::addObjectToPop(GameObject* object)
{

    this->m_MapPopulation[(*object).m_id] = object ;

    this->m_Population.push_back(object);

    this->m_YObjects[object] = m_Population.size();

    DEBUGPRINT("Total Population of Game Objects: %d\n", m_Population.size());
    
    //TODO: Sort robots at this point?
    
    sortPopulation();

    return 0;
}

int GridGame::removeObjectFromPop(GameObject* object)
{

    int returnval = removeObjectFromPop((*object).m_id);

    return returnval;

}

int GridGame::removeObjectFromPop(int objectid)
{

    GameObject* obj = m_MapPopulation[objectid];

    m_MapPopulation.erase( objectid );

    m_YObjects.erase( obj );
    
    std::vector<GameObject*>::iterator end = m_Population.end();
   
    for(std::vector<GameObject*>::iterator it = m_Population.begin(); it != end; it++)
    {
        if ((**it).m_id == objectid){
            printf("Found %d robot!\n", objectid);
            this->m_Population.erase(it);
            break;
        }

    }

    /*
    for(std::vector<Robot*>::iterator it = m_Population.begin(); it != end; it++)
    {
        if ((**it).m_id == (*robotobj).m_id){
            printf("Found %d robot!", robotid);
            this->m_Population.erase(it);
            break;
        }

    }*/

    return 0;

}

const float& GridGame::getWorldSize() const
{
    return m_WorldSize;
}

void GridGame::printPopulation(){

	for (int i = 0; i < m_Population.size(); i++){
	
        DEBUGPRINT("%d - ", i);
		m_Population[i]->printInfo();

	}
}

unsigned int readId(unsigned int id)
{
 /*   if ((id & PUCKVALUE)==PUCKVALUE)
    {
        unsigned int value = id || ROBOTVALUE;
        return value;
    }
    return id;
*/
}

unsigned int writeId(unsigned int id, int type)
{/*
    if (type == 1) // Then it is a puck
    {
        unsigned int value = (id || ROBOTVALUE);
        return value;
    }
    return id;
    */
}

