#include "Game/Game.h"
#include "Game/Robot.h"
#include "Game/Team.h"
#include "Math/Position.h"
#include "Math/Math.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <math.h>
using namespace Game;

Robotix* Robotix::m_Instance = NULL;

void Robotix::init(int argc, char** argv)
{
    //Seed our number generator.
    srand48(time(NULL));
    m_GamePaused = false;

    //Graphics details.
    m_ShowData = false;
    m_WindowSize = 600;
    
    //Robot configurations.
    Robot::m_FOV = Math::dtor(90.0);
    Robot::m_Radius = 0.01;
    Robot::m_SensorRange = 0.1;
    Robot::m_PickupRange = (Robot::m_SensorRange/5.0);

    //Home radius.
    Home::m_Radius = 0.1;
    
    m_WorldSize = 1.0;
    m_Updates = 0;
    m_MaxUpdates = 0;

    m_TotalPuckCount = 100;
    m_TotalTeamCount = 1;
    Team::m_RobotPopCount = 10000;
    
    m_SleepMsec = 10; 

    //parse options
    int c;
    while( ( c = getopt( argc, argv, "dh:a:p:s:f:r:u:z:w:")) != -1 )
	    switch( c )
	    {
	        case 'h':
	          m_TotalTeamCount = atoi( optarg );
	          printf( "[Antix] home count: %d\n", m_TotalTeamCount );
	          break;

	        case 'a':
	          m_TotalPuckCount = atoi( optarg );
	          printf( "[Antix] puck count: %d\n", m_TotalPuckCount );
	          break;
	          
	        case 'p': 
		        Team::m_RobotPopCount = atoi( optarg );
		        printf( "[Antix] home_population: %d\n", Team::m_RobotPopCount );
		        break;
		
	        case 's': 
		        m_WorldSize = atof( optarg );
		        printf( "[Antix] worldsize: %.2f\n", m_WorldSize );
		        break;
		
	        case 'f': 
		        Robot::m_FOV = dtor(atof( optarg )); // degrees to radians
		        printf( "[Antix] fov: %.2f\n", Robot::m_FOV );
		        break;
		
	        case 'r': 
		        Robot::m_SensorRange = atof( optarg );
		        printf( "[Antix] range: %.2f\n", Robot::m_SensorRange );
		        break;
						
	        case 'u':
		        m_MaxUpdates = atol( optarg );
		        printf( "[Antix] updates_max: %lu\n", (long unsigned)m_MaxUpdates );
		        break;
		
	        case 'z':
		        m_SleepMsec = atoi( optarg );
		        printf( "[Antix] sleep_msec: %d\n", m_SleepMsec );
		        break;
		
	        case 'w': 
                m_WindowSize = atoi( optarg );
		        printf( "[Antix] winsize: %d\n", m_WindowSize );
		        break;

	        case 'd': m_ShowData=true;
	          puts( "[Antix] show data" );
	          break;

            default:
                break;
	    }

    //Generate pucks at random locations.
    for (uint i = 0; i <m_TotalPuckCount; i++)
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
}

Robotix::~Robotix()
{
    //Note: The delete of the robot population is the responsiblity of the team object.
   
    //Delete the teams.
    for(std::list<Team*>::iterator it = m_Teams.begin(); it != m_Teams.end(); it++)
    {
        delete (*it);
    }

    //Delete the pucks.
    for(PuckIter it = m_Pucks.begin(); it != m_Pucks.end(); it++)
    {
        delete (*it);
    }
}

void Robotix::sortRobots()
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

void Robotix::update()
{
    //If we've set max update, then check if we've done enough updates.
    if (m_MaxUpdates > 0 && m_Updates > m_MaxUpdates)
        exit(1);

    //Main AI loop.
    if (!m_GamePaused)
    {
		//sort the robot population using insertion sort, which is good for
		//lists which do not get too out of order 
        sortRobots();

        //Update the position of all robots.
        for (RobotIter it = m_Population.begin(); it != m_Population.end(); it++)
        {
           (*it)->updatePosition(); 
        }

        std::vector<Robot*> l_Visible;
        for(unsigned int i = 0; i < m_XPos.size(); i++)
        {
            l_Visible.clear();
            
            //Get list of visible robots on x axis;
            std::vector<Robot*> l_XVisible;    
            for(int j = i-1; j >= 0 && ((m_XPos[i]->getPosition()->getX()-m_XPos[j]->getPosition()->getX()) <= Robot::getSensRange()) && fabs(m_XPos[j]->getPosition()->getY()-m_XPos[i]->getPosition()->getY()) <= Robot::getSensRange(); j--)
            {
               l_Visible.push_back(m_XPos[j]); 
            }
             for(unsigned int j = i+1; j < m_XPos.size() && ((m_XPos[j]->getPosition()->getX()-m_XPos[i]->getPosition()->getX()) <= Robot::getSensRange()) && fabs(m_XPos[j]->getPosition()->getY()-m_XPos[i]->getPosition()->getY()) <= Robot::getSensRange(); j++)
            {
               l_Visible.push_back(m_XPos[j]); 
            }

             //   printf("%d collisions\n", l_Visible.size());
             m_XPos[i]->updateSensors();
        }  

        //m_GamePaused = true;
        //Update the controller of all robots.
        for (RobotIter it = m_Population.begin(); it != m_Population.end(); it++)
        {
           (*it)->updateController(); 
        }

    }

    m_Updates++;

    if ( m_SleepMsec > 0 )
        usleep (m_SleepMsec*1e3);
}
void Robotix::addRobotToPop(Robot* robot)
{
    this->m_Population.push_back(robot);
    this->m_XPos.push_back(robot);
}
Robotix* Robotix::getInstance()
{
    if (!m_Instance)
    {
        m_Instance = new Robotix();
    }

    return m_Instance;
}

const unsigned int& Robotix::getWindowSize() const
{
    return m_WindowSize;
}

const float& Robotix::getWorldSize() const
{
    return m_WorldSize;
}

PuckIter Robotix::getFirstPuck()
{
    return m_Pucks.begin();
}

PuckIter Robotix::getLastPuck()
{
    return m_Pucks.end();
}

std::list<Team*>::iterator Robotix::getFirstTeam()
{
    return m_Teams.begin();
}

std::list<Team*>::iterator Robotix::getLastTeam()
{
    return m_Teams.end();
}

RobotIter Robotix::getFirstRobot()
{
    return m_Population.begin();
}

RobotIter Robotix::getLastRobot()
{
    return m_Population.end();
}
