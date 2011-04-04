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
using std::pair;
using std::make_pair;

#define MILLISECS_IN_SECOND 1000
#define PUCKVALUE 1000000000000000000000000000000
#define ROBOTVALUE 0000000000000000000000000000000
#define ROBOT 0
#define PUCK 1

typedef pair<int, GameObject*> ObjectIDPair;
typedef pair<int, std::vector<sensed_item> > ObjectPair;

GridGame::GridGame(int gridid, int num_of_teams, int robots_per_team, int id_from, int id_to, float homeRadius, float worldSize, int numGrids, int puckTotal)
{

    #ifdef DEBUG

    LOGPRINT("GRIDGAME STATUS:\t INIT Grid Id: %d, Num of teams: %d, robots per team: %d, idfrom: %d, idto: %d\n", gridid, num_of_teams, robots_per_team, id_from, id_to);

    #endif

    m_GridId = gridid;
    m_Num_Of_Teams = num_of_teams;
    m_Robots_Per_Team = robots_per_team;

    //Robot configurations.
    robot_FOV = Math::dtor(90.0);
    robot_Radius = 0.01;
    robot_SensorRange = 0.2;
    robot_PickupRange = robot_SensorRange/5.0;

    LOGPRINT("GRIDGAME STATUS:\t Robot FOV: \n%f\n", robot_FOV);


    //TODO: These values either will be read from config, or passed into constructor
    home_Radius = homeRadius;
    m_WorldSize = worldSize;
	m_NumGrids = numGrids;
    m_PuckTotal = puckTotal;
    
    LOGPRINT("GRIDGAME STATUS:\t INIT World info Home Radius:%f World Size:%f Numer of Grids:%i Puck Total%i\n", home_Radius, m_WorldSize, m_NumGrids, m_PuckTotal);

    // calculate boundary zones
    float max_grid_size = m_WorldSize/m_NumGrids;
    m_leftBoundary = (gridid - 1)*max_grid_size;
    m_rightBoundary = m_leftBoundary + max_grid_size;
    m_leftInnerBoundary = m_leftBoundary + robot_SensorRange; 
    m_rightInnerBoundary = m_rightBoundary - robot_SensorRange; 

    // determine the left and right grid ids
    m_LeftGrid = (m_GridId - 1 == 0) ? m_NumGrids : m_GridId - 1;
    m_RightGrid = (m_GridId + 1 > m_NumGrids) ? 1 : m_GridId + 1;

    LOGPRINT("GRIDGAME STATUS:\t INIT Left grid: %d Right grid: %d\n", m_LeftGrid, m_RightGrid);
    LOGPRINT("GRIDGAME STATUS:\t INIT Left boundary: %f Left inner boundary: %f\n", m_leftBoundary, m_leftInnerBoundary);
    LOGPRINT("GRIDGAME STATUS:\t INIT Right boundary: %f Right inner boundary: %f\n", m_rightBoundary, m_rightInnerBoundary);

    // Safety
    m_Population.clear();
    m_Teams.clear();

    // setting puck id ranges
    int l_pucks_per_grid = m_PuckTotal/m_NumGrids;
    int high_puck_range = l_pucks_per_grid * m_GridId;
    int low_puck_range = high_puck_range - l_pucks_per_grid + 1;

    //int high_puck_range = (m_PuckTotal/m_NumGrids)*(m_GridId+1);
    //int low_puck_range = (m_PuckTotal/m_NumGrids)*(m_GridId);

    Team* l_team;

    for (int i = id_from; i <= id_to; i++)
    {
        if (i%robots_per_team == 0)
        {
            //std::cout << "WTF2 id:" <<i << "robots per team: "<< robots_per_team << std::endl;
            Math::Position* l_RobotPosition = Math::Position::randomPosition(m_WorldSize,
                                                                             m_leftBoundary, 
                                                                             m_rightBoundary);
            l_team = new Team(l_RobotPosition, i/robots_per_team);
            m_Teams.push_back(l_team);
        }

        Math::Position* l_RobotPosition = Math::Position::randomPosition(m_WorldSize, 
                                                                         m_leftBoundary,
                                                                         m_rightBoundary);
                                                                         
        if (l_RobotPosition == NULL)
        {
        	ERRORPRINT("GRIDGAME ERROR:\t INIT Failed to get random position\n");
        }                                                                 
                                                                         
        Game::Robot* l_Robot = new Robot(l_RobotPosition, i );
        
        if (l_Robot == NULL)
        {
        	ERRORPRINT("GRIDGAME ERROR:\t INIT Failed to create robot with posistion\n");
        }
        
        addObjectToPop(l_Robot);
    }

    // TEMPORARY until I write the bit shifting functions
    high_puck_range = high_puck_range + 10000000;
    low_puck_range = low_puck_range + 10000000;

    LOGPRINT("GRIDGAME STATUS:\t INIT PUCK Range high:%d low:%d", high_puck_range, low_puck_range);


    //Generate pucks at random locations.
    for (uint i = low_puck_range; i <= high_puck_range; i++)
    {
       std::cout << i << std::endl;
       //Get a random location.
       Math::Position* l_PuckPos = Math::Position::randomPosition(m_WorldSize, 
                                                                  m_leftBoundary,
                                                                  m_rightBoundary);
       //Create the puck.
       Puck* l_Puck = new Puck(l_PuckPos, i);
       addObjectToPop(l_Puck);
    }

    for(std::vector<Team*>::iterator it = m_Teams.begin(); it != m_Teams.end(); it++)
    {
        LOGPRINT("GRIDGAME STATUS:\t INIT TEAM ID: %u Xpos:%f, Ypos:%f\n", (*it)->getId(), (*it)->getX(), (*it)->getY());
    }

    // set team and robot counters used by getRobots function
    teamcounter = 0;
    robotcounter = 0;
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
    DEBUGPRINT("GRIDGAME STATUS:\t Sort Population execution started\n");
    //Sort on the y axis.
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
    DEBUGPRINT("GRIDGAME STATUS:\t Sort Population execution finished\n");
    return;
}


int GridGame::getRobots(Msg_TeamInit& team, std::vector<Msg_InitRobot>* robots)
{

    DEBUGPRINT("GRIDGAME STATUS:\t Executing GETROBOTS() Team Counter: %d\n", teamcounter);
    Team* l_Team = m_Teams[teamcounter];
    team.id = l_Team->getId();
    team.x = l_Team->getX();
    team.y = l_Team->getY();

    DEBUGPRINT("GRIDGAME STATUS:\t Team id: %d, Team x: %f, Team y: %f\n", team.id, team.x, team.y);

	DEBUGPRINT("GRIDGAME STATUS:\t Robo Counter Min:%d, Robot Counter Max: %d\n", robotcounter, robotcounter+m_Robots_Per_Team);
    for(int i = robotcounter; i < robotcounter+m_Robots_Per_Team; i++)
    {
        GameObject* l_Robot = m_Population[i];
        
        if (l_Robot == NULL) return -1;
        
        Msg_InitRobot temp;
        temp.id = l_Robot->getId();
        temp.x = l_Robot->getX();
        temp.y = l_Robot->getY();
		DEBUGPRINT("GRIDGAME STATUS:\t GETROBOTS() ID:%d, Xpos:%f, Ypos:%f\n", temp.id, temp.x, temp.y);
        robots->push_back(temp);
    }

    // increment our counts
    teamcounter++;
    robotcounter += m_Robots_Per_Team;


    // If we are finished distributing all of the robots to the clients, sort the population
    int total_robots = m_Robots_Per_Team*m_Num_Of_Teams;
    DEBUGPRINT("GRIDGAME STATUS:\t Total robots: %d\n", total_robots);
    DEBUGPRINT("GRIDGAME STATUS:\t Robot counter: %d\n", robotcounter);
    if (robotcounter == total_robots)
    {
       // sortPopulation();

    }

    return 0;
}

bool GridGame::robotsDepleted()
{
    return (m_Robots_Per_Team*m_Num_Of_Teams == robotcounter)?true:false;
}

int GridGame::registerRobot(Msg_RobotInfo robot)
{
    // create a new position object for the new robot
    Math::Position* l_RobotPosition = new Position(robot.x_pos, robot.y_pos, robot.angle);

	if (l_RobotPosition == NULL)
	{
		ERRORPRINT("GRIDGAME ERROR:\t Failed to allocate memory in registerRobot()\n");
		return -1;
	}

    // create new robot based on info from robot_info struct
    Game::Robot* l_Robot = new Robot(l_RobotPosition, robot.robotid);
    
    if (l_Robot == NULL)
    {
    	ERRORPRINT("GRIDGAME ERROR:\t Failed to allocate memory for robot in registerRobot()\n");
    	return -1;
    }
    
    (*l_Robot).m_PuckHeld = robot.puckid;
			
    // add robot to the population
    addObjectToPop(l_Robot);

    //Sort robots at this point
    //sortPopulation();

    return 0;
    
}

int GridGame::unregisterRobot(unsigned int robotid)
{
    int removeResult = removeObjectFromPop(robotid);
    return removeResult;
}

int GridGame::returnSensorData(std::vector<uid>& robot_ids_from_client,
                               std::vector< RobotSensedObjectsPair >* sensor_data,
                               int& totalSensed)
{
    DEBUGPRINT("GRIDGAME STATUS:\t Entering returnSensorData()\n");

#ifdef DEBUG
    printPopulation();
#endif
    
    std::vector<uid>::iterator clientend = robot_ids_from_client.end();
    for(std::vector<uid>::iterator it = robot_ids_from_client.begin(); it != clientend; it++)
    {
        uid robotId = (*it);
        DEBUGPRINT("GRIDGAME STATUS:\t Getting sensor data for RobotId:%d\n", robotId);
        // grab GameObject from int id
        GameObject* tempobj = m_MapPopulation[robotId];
        
        if(tempobj == NULL)
        {
            ERRORPRINT("GRIDGAME ERROR:\t RobotID %d not found in population, exiting.\n", robotId);
            return -1;
        }

        // grab vector position in sorted population
        int position = m_YObjects[tempobj];

        //printf("position for gameobject id %d: %d\n", (*tempobj).m_id, position); 

        GameObject* position_obj = m_Population[position];

		if (position_obj == NULL)
		{
			ERRORPRINT("GRIDGAME ERROR:\t Failed to find position data\t");
			return -1;
		}

        std::vector<Msg_SensedObjectGroupItem> temp_vector;

        int counter = position;

        while ( counter >= 0 )
        {
            if ( (tempobj->getY() - position_obj->getY()) < robot_SensorRange )
            {
                if ( abs(tempobj->getX() - position_obj->getX()) < robot_SensorRange )
                {
                    Msg_SensedObjectGroupItem temp_sensed;
                    temp_sensed.id = (*position_obj).m_id;
                    temp_sensed.x = position_obj->getX();
                    temp_sensed.y = position_obj->getY();

                    // if the game object found in the sensor range is the same
                    // as the robot we are looking for, do not add the same robot to
                    // its own sensed objects
                    if (tempobj->getId() != position_obj->getId())
                    {
                        temp_vector.push_back( temp_sensed );
                        totalSensed++;
                    }
                    
                }
                counter--;
                position_obj = m_Population[counter];
            }
            else
            {
                break;
            }
        }

        counter = position;
        position_obj = m_Population[position];

        //DEBUGPRINT("Counter: %d - Size client list: %zu\n", counter, robot_ids_from_client.size() );

        while ( counter < m_Population.size() )
        {
            if ( (position_obj->getY() - tempobj->getY()) < robot_SensorRange )
            {
                if ( abs(position_obj->getX() - tempobj->getX()) < robot_SensorRange )
                {
                    Msg_SensedObjectGroupItem temp_sensed;
                    temp_sensed.id = (*position_obj).m_id;
                    temp_sensed.x = position_obj->getX();
                    temp_sensed.y = position_obj->getY();

                    // if the game object found in the sensor range is the same
                    // as the robot we are looking for, do not add the same robot to
                    // its own sensed objects
                    if (tempobj->getId() != position_obj->getId())
                    {
                        temp_vector.push_back( temp_sensed );
                        totalSensed++;
                    }
                }
                counter++;
                position_obj = m_Population[counter];
            }
            else
            {
                break;
            }
        }
        
        sensor_data->push_back(RobotSensedObjectsPair((*it), temp_vector));
    }
    
    return 0;
}

int GridGame::processAction(std::vector<Msg_Action>& robot_actions, std::vector< Msg_RobotInfo >* results, std::vector<std::pair<int, std::vector<Msg_RobotInfo> > >* robots_to_pass)
{

    // basic movement, not checking collisons
    // not even checking the angle sent by the client.
    // check to see if the robot is in the boundary zone, if they are add them to robots_to_pass

    if(robots_to_pass->size() == 0)
    {
        DEBUGPRINT("GRIDGAME STATUS:\t No robots to pass\n");
        RobotInfoList left_robots;
        RobotInfoList right_robots;
        std::pair<int, RobotInfoList > left_robots_pair = std::make_pair(0, left_robots);
        std::pair<int, RobotInfoList > right_robots_pair = std::make_pair(1, right_robots);
        robots_to_pass->push_back(left_robots_pair);
        robots_to_pass->push_back(right_robots_pair);
    }

    RobotInfoList* left_robots = &robots_to_pass->at(0).second;
    RobotInfoList* right_robots = &robots_to_pass->at(1).second;
    
    left_robots->clear();
    right_robots->clear();

    // iterate through each robot recieved by grabbing that robot from our population array
    for(std::vector<Msg_Action>::iterator it = robot_actions.begin(); it != robot_actions.end(); it++)
    {
        // grab the robots and update their positions
        DEBUGPRINT("GRIDGAME STATUS:\t Looking for robot: %d\n", (*it).robotid);
        std::tr1::unordered_map<int, GameObject*>::iterator robot_find = m_MapPopulation.find((*it).robotid);
        if (robot_find != m_MapPopulation.end())
        {
            Robot* l_Robot = (Robot*)m_MapPopulation[(*it).robotid];
            Msg_RobotInfo temp;
            temp.robotid = l_Robot->getId();
            temp.x_pos = l_Robot->getX();
            DEBUGPRINT("GRIDGAME STATUS:\t Previous Xpos: %f Ypos: %f\n", l_Robot->getX(), l_Robot->getY());
            
            // TODO: Need to handle robots wrapping around both left and right, up and down
            float old_ypos = l_Robot->getY();
            float new_ypos = old_ypos;
            if (new_ypos > m_WorldSize)
            {
                new_ypos -= m_WorldSize;
            }
            
            float old_xpos = l_Robot->getX();
            float new_xpos = old_xpos + 0.1;
            if (new_xpos > m_WorldSize)
            {
                new_xpos -= m_WorldSize;
            }
            temp.x_pos = new_xpos;
            temp.y_pos = new_ypos;
            temp.speed = (*it).speed;
            temp.angle = (*it).angle;
            temp.puckid = 0;
            DEBUGPRINT("GRIDGAME STATUS:\t New position Xpos: %f Ypos: %f\n", temp.x_pos, temp.y_pos);

            // set the new position of the robot
            l_Robot->setPosition(temp.x_pos, temp.y_pos, temp.angle);

            // check if the robots new position is in a new grid. If it is, we must remove the robot
            // from the grids population, and set the gridid in the Msg_RobotInfo, so the robotclient
            // can fix the mapping
            if( outOfBoundsRight(new_xpos) )
            {
                // set the robot to the appropriate grid
                temp.gridid = m_RightGrid;
                // remove robot from population
                removeObjectFromPop(temp.robotid);

                DEBUGPRINT("GRIDGAME STATUS:\t Robot has left this grid on the right side! Removing it from population\n");

            } //&& !(temp.x_pos < (0.0f + robot_SensorRange)) 
            else if( outOfBoundsLeft(new_xpos) )
            {
                // set the robot to the appropriate grid
                temp.gridid = m_LeftGrid;
                //remove robot from population
                removeObjectFromPop(temp.robotid);

                DEBUGPRINT("GRIDGAME STATUS:\t Robot has left this grid on the left side! Removing it from population\n");
            }
            else
            {
                temp.gridid = m_GridId;
            }

            // add the robot to the results (which will be processed by the client who requested
            results->push_back(temp);

            if(m_NumGrids != 1)
            {
                // check if robots are in the boundary zone
                if( inRightInnerBoundary(new_xpos) || outOfBoundsRight(new_xpos) )
                {
                    right_robots->push_back(temp);
                    DEBUGPRINT("GRIDGAME STATUS:\t Sending robotid %d to the right grid", temp.robotid);
                }
                else if( inLeftInnerBoundary(new_xpos) || outOfBoundsLeft(new_xpos) )
                {
                    left_robots->push_back(temp);
                    DEBUGPRINT("GRIDGAME STATUS:\t Sending robotid %d to the left grid", temp.robotid);
                }
            
                // Check to see if robot was in boundary zone, and then left. If so, tell neighbor that robot
                // is now gone.
                if( inLeftInnerBoundary(old_xpos) && inMidZone(new_xpos) )
                {
                    temp.gridid = 0;
                    left_robots->push_back(temp);
                }
                else if( inRightInnerBoundary(old_xpos) && inMidZone(new_xpos) )
                {
                    temp.gridid = 0;
                    right_robots->push_back(temp);
                }
            }
        }
        else
        {
            DEBUGPRINT("GRIDGAME ERROR:\t Robot is not in the population");
            return -1;
        }
    }

    // sort population after we update the positions
    //sortPopulation();

    return 0;
}

bool GridGame::outOfBoundsLeft(float x_pos)
{
    if(m_GridId == 1)
    {
        if(x_pos > m_WorldSize - robot_SensorRange)
        {
            return true;
        }
    }
    else
    {
        if(x_pos < m_leftBoundary)
        {
            return true;
        }
    }
    return false;
}

bool GridGame::outOfBoundsRight(float x_pos)
{
    if(m_GridId == m_NumGrids)
    {
        if(x_pos < 0.0f + robot_SensorRange)
        {
            return true;
        }
    }
    else
    {
        if(x_pos > m_rightBoundary)
        {
            return true;
        }
    }
    return false;
}

bool GridGame::inLeftInnerBoundary(float x_pos)
{
    return (x_pos > m_leftBoundary && x_pos < m_leftInnerBoundary);
}

bool GridGame::inRightInnerBoundary(float x_pos)
{
    return (x_pos < m_rightBoundary && x_pos > m_rightInnerBoundary);
}

bool GridGame::inMidZone(float x_pos)
{
    return ( x_pos > m_leftInnerBoundary && x_pos < m_rightInnerBoundary );
}

int GridGame::updateRobots(RobotInfoList& robots)
{
    DEBUGPRINT("GRIDGAME STATUS:\t Entering updateRobot function\n");

    // iterate through each robot recieved by grabbing that robot from our population array
    for(std::vector<Msg_RobotInfo>::iterator it = robots.begin(); it != robots.end(); it++)
    {
        DEBUGPRINT("GRIDGAME STATUS:\t Entering updateRobot function for loop\n");
        // grab the robots and update their positions
        std::tr1::unordered_map<int, GameObject*>::iterator robot_find = m_MapPopulation.find((*it).robotid);
        if (robot_find == m_MapPopulation.end())
        {
            DEBUGPRINT("GRIDGAME STATUS:\t NEW ROBOT\n");
            registerRobot(*it);
        }
        else
        {
            if((*it).gridid == 0)
            {
                DEBUGPRINT("GRIDGAME STATUS:\t REMOVE ROBOT ID:%d\n", (*it).robotid);
                removeObjectFromPop((*it).robotid);
            }
            else
            {
                DEBUGPRINT("GRIDGAME STATUS:\t UPDATE ROBOT ID:%d\n", (*it).robotid);
                Robot* l_Robot = (Robot*)m_MapPopulation[(*it).robotid];
                l_Robot->updatePosition((*it).x_pos, (*it).y_pos);
                l_Robot->m_PuckHeld = (*it).puckid;
                l_Robot->getPosition()->setOrient((*it).angle);
                DEBUGPRINT("GRIDGAME STATUS:\t MOVED TO: %f\n", l_Robot->getPosition()->getX());
            }
        }
    }
    //sortPopulation();
    return 0;
}

int GridGame::addObjectToPop(GameObject* object)
{
    this->m_MapPopulation[(*object).m_id] = object ;
    this->m_Population.push_back(object);
    this->m_YObjects[object] = m_Population.size();

    //DEBUGPRINT("Total Population of Game Objects: %zu\n", m_Population.size());
    //printPopulation();
    
    //TODO: Sort robots at this point?
    
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
            DEBUGPRINT("GRIDGAME STATUS:\t Removing robot ID:%d from the population\n", objectid);
            this->m_Population.erase(it);
            break;
        }
    }

    return 0;
}

int GridGame::getPopulation(std::vector< Msg_RobotInfo >* results, float top, float bottom, float left, float right)
{
    std::vector<GameObject*>::iterator endit = m_Population.end();
    for(std::vector<GameObject*>::iterator it = m_Population.begin(); it != endit; it++)
    {   
    	//only send robots within view
        float l_x = (**it).getX();
        float l_y = (**it).getY();
        if (l_x > right || l_x < left || l_y > top || l_y < bottom) continue;
        
        Msg_RobotInfo l_ObjInfo;
        l_ObjInfo.robotid = Antix::writeId((**it).getId(), ROBOT);
        l_ObjInfo.x_pos = -1.0;
        l_ObjInfo.y_pos = -1.0;
        l_ObjInfo.angle = 0.0;
        l_ObjInfo.puckid = 0;        
        
        //send reset values if the robot is out of bounds, otherwise, send real position data
        if (!outOfBoundsLeft(l_x) && !outOfBoundsRight(l_x))
        {
		    l_ObjInfo.x_pos = l_x;
		    l_ObjInfo.y_pos = l_y;
		    l_ObjInfo.angle = ((**it).getPosition())->getOrient();
		    //l_ObjInfo.puckid = (**it).m_PuckHeld;
        }

        results->push_back(l_ObjInfo);
    }
    
    DEBUGPRINT("GRIDGAME STATUS:\t Done pushing robots to drawer\n");
    return 0;
}

int GridGame::getTeams(std::vector< Msg_TeamInit >* results)
{
    std::vector<Team*>::iterator endit = m_Teams.end();
    
    for(std::vector<Team*>::iterator it = m_Teams.begin(); it != endit; it++)
    {   
        Msg_TeamInit l_TeamInfo;

        // for each object being pushed
        l_TeamInfo.id = (**it).m_TeamId;
        l_TeamInfo.x = (**it).getX();
        l_TeamInfo.y = (**it).getY();

        results->push_back(l_TeamInfo);
    }
    DEBUGPRINT("GRIDGAME STATUS:\t Done pushing teams to drawer\n");
    return 0;
}

const float& GridGame::getWorldSize() const
{
    return m_WorldSize;
}

const float& GridGame::getLeftBoundary() const
{
    return m_leftBoundary;
}

const float& GridGame::getRightBoundary() const
{
    return m_rightBoundary;
}

void GridGame::printPopulation()
{
    DEBUGPRINT("GRIDGAME STATUS:\t Population size:%d\n", m_Population.size());
	for (int i = 0; i < m_Population.size(); i++){	
		m_Population[i]->printInfo();
	}
	return;
}

