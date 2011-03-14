#include "GridGame.h"
#include "Robot.h"
#include "Team.h"
#include "Position.h"
#include "MathAux.h"


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

GridGame::GridGame()
{

    //Robot configurations.
    robot_FOV = Math::dtor(90.0);
    robot_Radius = 0.01;
    robot_SensorRange = 0.1;
    robot_PickupRange = robot_SensorRange/5.0;

    printf("\n%d\n", robot_FOV);

    //Home radius.
    home_Radius = 0.1;

    m_WorldSize = 10;
	m_NumGrids = 2;
    m_GridId = 1;

    m_PuckTotal = 10000;

    m_Population.clear();

    int high_puck_range = (m_PuckTotal/m_NumGrids)*m_GridId;
    int low_puck_range = (m_PuckTotal/m_NumGrids)*(m_GridId-1);
    
    //Generate pucks at random locations.
    for (uint i = low_puck_range; i <= high_puck_range; i++)
    {
       //Get a random location.
       Math::Position* l_PuckPos = Math::Position::randomPosition(m_WorldSize, m_NumGrids, m_GridId);

       //Create the puck.
       Puck* l_Puck = new Puck(l_PuckPos, i);
       m_Pucks.push_back( l_Puck );
    }

    /*
  
    //Generate the teams. 
    for (uint i = 0; i < m_TotalTeamCount; i++)
    {
       Team* l_Team = new Team();
       m_Teams.push_back( l_Team ); 
    }

    if (m_loggingEnabled)
    {
        std::ostringstream s;
        s << "Home=" << m_TotalTeamCount << ", Population=" << Team::m_RobotPopCount;
    }

    */

}

GridGame::~GridGame()
{
    //Note: The delete of the robot population is the responsiblity of the team object.
   
    //Delete the teams.
    //for(std::vector<Team*>::iterator it = m_Teams.begin(); it != m_Teams.end(); it++)
    //{
    //    delete (*it);
    //}

    //Delete the pucks.
    for(PuckIter it = m_Pucks.begin(); it != m_Pucks.end(); it++)
    {
        delete (*it);
    }
}

void GridGame::sortRobots()
{
    //Sort on the x axis.
    int i;
    Robot* l_Key;
    for(unsigned int j = 1; j < m_XPos.size(); j++)
    {
        l_Key = m_XPos[j];
        i = j - 1;
        while(i >= 0 && m_XPos[i]->getPosition()->getX() > l_Key->getPosition()->getX())
        {
            m_XPos[i+1] =m_XPos[i];
            m_XRobs[m_XPos[i+1]] = i+1;
            --i;
        }

        m_XPos[i+1] = l_Key;
        m_XRobs[l_Key] = i+1;
    }
}

int GridGame::randomizeTeam(int team_id, int team_size, std::vector<robot_info>* robot_info_vector)
{

        // get range of robot ids based on team id and team size
		int firstid = team_id*team_size;
		int lastid = firstid + team_size - 1;

        std::vector<robot_info> l_robot_info_vector;

		for (int i = firstid; i <= lastid; i++)
		{
			Math::Position* l_RobotPosition = Math::Position::randomPosition(m_WorldSize, m_NumGrids, m_GridId);
       		Game::Robot* l_Robot = new Robot(l_RobotPosition, i, robot_FOV, robot_Radius, robot_PickupRange, robot_SensorRange);
			addRobotToPop(l_Robot);

            robot_info temp;
            temp.id = i;
            temp.x_pos = l_RobotPosition->getX();
            temp.y_pos = l_RobotPosition->getY();

            l_robot_info_vector.push_back(temp);
		}
        robot_info_vector = &l_robot_info_vector;
}

int GridGame::registerRobot(robot_info robot)
{
    // create a new position object for the new robot
    Math::Position* l_RobotPosition = new Position(robot.x_pos, robot.y_pos, robot.angle);

    // create new robot based on info from robot_info struct
    Game::Robot* l_Robot = new Robot(l_RobotPosition, robot.id, robot_FOV, robot_Radius, robot_PickupRange, robot_SensorRange);
			
    // add robot to the population
    addRobotToPop(l_Robot);

    //TODO: Sort robots at this point?

}

int GridGame::unregisterRobot(unsigned int robotid)
{

    std::vector<Robot*>::iterator end = m_Population.end();
   
    for(std::vector<Robot*>::iterator it = m_Population.begin(); it != end; it++)
    {
        if ((**it).m_id == robotid){
            printf("Found %d robot!", robotid);
            this->m_Population.erase(it);
        }

    }

    //TODO: Sort robots at this point?

}

int GridGame::returnSensorData(std::vector<int> robot_ids_from_client, std::map<int, std::vector<sensed_item> >* sensed_items_map)
{

    std::vector<Robot*>::iterator populationend = m_Population.end();
    std::vector<Puck*>::iterator puckend = m_Pucks.end();
    // TODO: Currently returning all items on grid. Will be optimized to only send what is around
    //       its bounding box
    std::vector<sensed_item>  l_sensed_items;
    for(std::vector<Robot*>::iterator robotit = m_Population.begin(); robotit != populationend; robotit++){
                sensed_item tempsensed;
                tempsensed.id = (**robotit).m_id;
                tempsensed.x = (*robotit)->getX();
                tempsensed.y = (*robotit)->getY();
                l_sensed_items.push_back(tempsensed);
    }
    for(std::vector<Puck*>::iterator puckit = m_Pucks.begin(); puckit != puckend; puckit++){
                sensed_item tempsensed;
                tempsensed.id = (**puckit).id;
                tempsensed.x = (*puckit)->getX();
                tempsensed.y = (*puckit)->getY();
                l_sensed_items.push_back(tempsensed);
    }
    

    // TODO: Implement with hash key to robot value being index of robot population in vector
    //       that way, we don't have to iterate through the nested forloop to find the robots
    typedef std::pair<int, std::vector<sensed_item> > RobotPair;
    std::map<int, std::vector<sensed_item> > l_sensed_items_map;
  

    std::vector<int>::iterator clientend = robot_ids_from_client.end();

    for(std::vector<int>::iterator it = robot_ids_from_client.begin(); it != clientend; it++)
    {
        for(std::vector<Robot*>::iterator robotit = m_Population.begin(); robotit != populationend; robotit++)
            if ( (**robotit).m_id == (*it)  )
            {
                l_sensed_items_map.insert(RobotPair( (**robotit).m_id , l_sensed_items ));
            }
    }

    sensed_items_map = &l_sensed_items_map;

}



void GridGame::addRobotToPop(Robot* robot)
{

    this->m_Population.push_back(robot);
    this->m_XPos.push_back(robot);
}

const float& GridGame::getWorldSize() const
{
    return m_WorldSize;
}

PuckIter GridGame::getFirstPuck()
{
    return m_Pucks.begin();
}

PuckIter GridGame::getLastPuck()
{
    return m_Pucks.end();
}

RobotIter GridGame::getFirstRobot()
{
    return m_Population.begin();
}

RobotIter GridGame::getLastRobot()
{
    return m_Population.end();
}

void GridGame::printRobotPopulation(){

	for (int i = 0; i < m_Population.size(); i++){
		
		m_Population[i]->printInfo();

	}
}

