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

    #ifdef DEBUG

    printf("Gridid: %d, Num of teams: %d, robots per team: %d, idfrom: %d, idto: %d", gridid, num_of_teams, robots_per_team, id_from, id_to);

    #endif

    m_GridId = gridid;
    m_Num_Of_Teams = num_of_teams;
    m_Robots_Per_Team = robots_per_team;

    //Robot configurations.
    robot_FOV = Math::dtor(90.0);
    robot_Radius = 0.01;
    robot_SensorRange = 4;
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
        //std::cout << "Robot id being created:" <<i << std::endl;
       // if you can divide the id by the robots per team, that means that this is a team, create a new team
       // object for the rest of the robots to point to

        //std::cout << "WTF id:" <<i << "robots per team: "<< robots_per_team << std::endl;
        #endif
        if (i%robots_per_team == 0)
        {
            //std::cout << "WTF2 id:" <<i << "robots per team: "<< robots_per_team << std::endl;
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
    //sortPopulation(); // commented out as we should not sort the population until the getRobot function is
    //                     run enough times that all of the clients have all of the robots

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

    DEBUGPRINT("***Sort Population executing**\n");
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


int GridGame::getRobots(Msg_TeamInit& team, std::vector<Msg_InitRobot>* robots)
{

    DEBUGPRINT("===Executing getRobots");

    Team* l_Team = m_Teams[teamcounter];
    team.id = l_Team->getId();
    team.x = l_Team->getX();
    team.y = l_Team->getY();

    DEBUGPRINT("Team id: %d - Team x: %d - Team y: %d", team.id, team.x, team.y);

    //std::vector<robot_info> l_robot_info_vector = new std::vector<robot_info>();

    int i;
	printf("robo counter: %d, robotconter+m_Robos/team: %d\n", robotcounter, robotcounter+m_Robots_Per_Team);
    for (i = robotcounter; i < robotcounter+m_Robots_Per_Team; i++)
    {
        GameObject* l_Robot = m_Population[i];
        Msg_InitRobot temp;
        temp.id = l_Robot->getId();
        temp.x = l_Robot->getX();
        temp.y = l_Robot->getY();
		DEBUGPRINT("ID: %d, X: %f, F: %f \n", temp.id, temp.x, temp.y);

        robots->push_back(temp);
    }


    // increment our countes
    teamcounter++;
    robotcounter += m_Robots_Per_Team;


    // If we are finished distributing all of the robots to the clients, sort the population
    int total_robots = m_Robots_Per_Team*m_Num_Of_Teams;
    DEBUGPRINT("Total robots: %d\n", total_robots);
    DEBUGPRINT("Robot counter: %d\n", robotcounter);
    if (robotcounter == total_robots)
    {
        sortPopulation();

    }


}

bool GridGame::robotsDepleted()
{

    
    return (m_Robots_Per_Team*m_Num_Of_Teams == robotcounter)?true:false;

}

int GridGame::registerRobot(Msg_RobotInfo robot)
{
    // create a new position object for the new robot
    Math::Position* l_RobotPosition = new Position(robot.x_pos, robot.y_pos, robot.angle);

    // create new robot based on info from robot_info struct
    Game::Robot* l_Robot = new Robot(l_RobotPosition, robot.robotid);
    (*l_Robot).m_PuckHeld = robot.puckid;
			
    // add robot to the population
    addObjectToPop(l_Robot);

    //Sort robots at this point
    sortPopulation();

    return 1;
    
}

int GridGame::unregisterRobot(unsigned int robotid)
{

    int removeResult = removeObjectFromPop(robotid);    


    return removeResult;



}

///int GridGame::returnSensorData(std::vector<int> robot_ids_from_client, std::map<int, std::vector<sensed_item> >* sensed_items_map)
int GridGame::returnSensorData(std::vector<int> robot_ids_from_client, std::vector< RobotSensedObjectsPair >* sensor_data)
{

    for( std::map<GameObject*, int>::iterator it = m_YObjects.begin(); it != m_YObjects.end(); it++){

        std::cout << it->first << " => " << it->second << std::endl;

    }

    //std::map<int, std::vector<sensed_item> > l_sensed_items_map; 

    std::vector<int>::iterator clientend = robot_ids_from_client.end();
    for(std::vector<int>::iterator it = robot_ids_from_client.begin(); it != clientend; it++)
    {
        // grab GameObject from int id
        GameObject* tempobj = m_MapPopulation[(*it)]; 

        // grab vector position in sorted population
        int position = m_YObjects[tempobj];

        printf("position for gameobject id %d: %d\n", (*tempobj).m_id, position); 

        GameObject* position_obj = m_Population[position];

        std::vector<Msg_SensedObjectGroupItem> temp_vector;

        int counter = position;

        while ( counter >= 0 )
        {
            if ( (tempobj->getY() - position_obj->getY()) < robot_SensorRange ){
                //TODO: Check Y bound as well

                if ( abs(tempobj->getX() - position_obj->getX()) < robot_SensorRange ){
                    Msg_SensedObjectGroupItem temp_sensed;
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

        DEBUGPRINT("Counter: %d - Size client list: %zu\n", counter, robot_ids_from_client.size() );

        while ( counter <= m_Population.size() )
        {
            if ( (position_obj->getY() - tempobj->getY()) < robot_SensorRange ){
                //TODO: Check Y bound as well

                if ( abs(position_obj->getX() - tempobj->getX()) < robot_SensorRange ){
                    Msg_SensedObjectGroupItem temp_sensed;
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

        //l_sensed_items_map[(*it)] = temp_vector;
        //sensed_items.push_back(RobotSensedObjectsPair<(*it), temp_vector>);
        sensor_data->push_back(RobotSensedObjectsPair((*it), temp_vector));
       
        /*
        #ifdef DEBUG
        int total = 0;

        for( std::vector<sensed_item>::iterator someit = temp_vector.begin(); someit != temp_vector.end(); someit++ ){
            std::cout << (*someit).id << ",";
            total++;
        }
        std::cout << "Total:" <<total << std::endl;
        #endif
        */

    }

    //sensed_items_map = &l_sensed_items_map;

    
    #ifdef DEBUG
    //for( std::map<int, std::vector<sensed_item> >::iterator it = l_sensed_items_map.begin(); it != l_sensed_items_map.end(); it++){
    for( std::vector< RobotSensedObjectsPair >::iterator it = sensor_data->begin(); it != sensor_data->end(); it++){

        std::cout << "robot id:" << it->first << " => "; 
        
        for( std::vector<Msg_SensedObjectGroupItem>::iterator iit = (it->second).begin(); iit != (it->second).end(); iit++){
            std::cout << (*iit).id << ",";
        }
        std::cout << std::endl;

    }
    #endif
    

    return 1;

}

int GridGame::processAction(std::vector<Msg_Action>& robot_actions, std::vector< Msg_RobotInfo >* results)
{

    // basic movement, moving up and down, not checking collisons
    // not even checking the angle sent by the client.

    // NOT TESTED!

    for(std::vector<Msg_Action>::iterator it = robot_actions.begin(); it != robot_actions.end(); it++)
    {
        Robot* l_Robot = (Robot*)m_MapPopulation[(*it).robotid];
        Msg_RobotInfo temp;
        temp.robotid = l_Robot->getId();
        temp.x_pos = l_Robot->getX();
        float new_ypos = l_Robot->getY() + 0.1;
        if (new_ypos > m_WorldSize)
        {
            new_ypos = 0;
        }
        temp.y_pos = new_ypos;
        temp.speed = (*it).speed;
        temp.angle = (*it).angle;
        temp.puckid = 0;

        l_Robot->setPosition(temp.x_pos, temp.y_pos, temp.angle);

        results->push_back(temp);
    }

    return 0;
}


int GridGame::addObjectToPop(GameObject* object)
{

    this->m_MapPopulation[(*object).m_id] = object ;

    this->m_Population.push_back(object);

    this->m_YObjects[object] = m_Population.size();

    DEBUGPRINT("Total Population of Game Objects: %zu\n", m_Population.size());
    
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

int GridGame::getPopulation(std::vector< Msg_RobotInfo >* results)
{

    std::vector<GameObject*>::iterator endit = m_Population.end();


    for(std::vector<GameObject*>::iterator it = m_Population.begin(); it != endit; it++)
    {   
        // Computes robots altered position (Random)
        //robotPosition[i][0] += float((rand()%200)-100)/50;
        //robotPosition[i][1] += float((rand()%200)-100)/50;

        //float posX = robotPosition[i][0];
        //float posY = robotPosition[i][1];
        //float orientation = 1.0;
        Msg_RobotInfo l_ObjInfo;

        // for each object being pushed
        l_ObjInfo.robotid = Antix::writeId((**it).getId(), ROBOT);
        l_ObjInfo.x_pos = (**it).getX();
        l_ObjInfo.y_pos = (**it).getY();
        l_ObjInfo.angle = ((**it).getPosition())->getOrient();
        //l_ObjInfo.puckid = (**it).m_PuckHeld; //TODO: Fix for later
        l_ObjInfo.puckid = 0;

        results->push_back(l_ObjInfo);
    }


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
