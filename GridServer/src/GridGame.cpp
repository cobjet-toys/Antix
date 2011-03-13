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
    WorldSize = 1.0;

    /*
    //Generate pucks at random locations.
    for (uint i = 0; i <robot_TotalPuckCount; i++)
    {
       //Get a random location.
       Math::Position* l_PuckPos = Math::Position::randomPosition(m_WorldSize);

       //Create the puck.
       Puck* l_Puck = new Puck(l_PuckPos);
       m_Pucks.push_back( l_Puck );
    }
  
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
    for(std::vector<Team*>::iterator it = m_Teams.begin(); it != m_Teams.end(); it++)
    {
        delete (*it);
    }

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
