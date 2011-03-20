#include "RobotGame.h"
#include "Team.h"
#include "MathAux.h"
#include "Types.h"
#include "Config.h"
#include "AntixUtil.h"
#include <iostream>

using std::cout;
using std::endl;

RobotGame::RobotGame()
{
    //TODO: these should come out of the config
    robot_FOV = Math::dtor(90.0);
    robot_Radius = 0.01;
    robot_SensorRange = 0.1;
    robot_PickupRange = robot_SensorRange / 5.0;

}

RobotGame::~RobotGame()
{
}

// Compile grid to team mapping
/*
int RobotGame::intitializeTeam(int grid_id, std::vector<int> team_mapping)
{
    // we know which teams are on which grid
    // so, create a mapping of grid ids to a vector of team ids

    return 0;
}
*/

/*
int RobotGame::receiveInitialRobots(int grid_id, std::vector<robot_info> robot_info_vector)
{
    

    std::vector<Robot*> l_Robots;
    std::vector<robot_info>::const_iterator end = robot_info_vector.end();
    for(std::vector<robot_info>::const_iterator it = robot_info_vector.begin(); it != end; it++)
    {
        Math::Position* l_RobotPosition = new Math::Position(it->x_pos, it->y_pos, it->angle);
        Game::Robot* l_Robot = new Robot(l_RobotPosition, it->id, robot_FOV, robot_Radius, robot_SensorRange, robot_PickupRange);
        l_Robots.push_back(l_Robot);

        l_Robot->printInfo();
    }
    m_Robots[grid_id] = l_Robots;

    return 0;
}
*/

int RobotGame::initTeam(int id, float x, float y)
// Creates a home with id at x,y
{
    m_homeLocations[id] = std::make_pair(x,y);
    //printf("added mappign for team, id: %d, location: %f,%f\n", id, m_homeLocations[id].first, m_homeLocations[id].second);
    return 0;
}

int RobotGame::setTeamRobot(int gridId, int teamId, int robotId, float x, float y)
// Creates a robot and adds it to the grid to robot vector mapping. Creates the robot at position x,y, and team teamId.
{
    Math::Position* l_robotPosition = new Math::Position(x,y, 0.0); // initial angle is 0.0
    Game::Robot* l_Robot = new Robot(l_robotPosition, teamId, robotId);
    m_robotsByGrid[gridId].push_back(l_Robot);

    return 0;
}

// Register and UnRegister robots from a particular grid
int RobotGame::registerRobot(int grid_id, robot_info robot)
{
    // this will be called when a grid wants to register a robot
    // add the robot to the right grid in the map

    Speed* l_Speed = new Speed();
    l_Speed->setForwSpeed(robot.speed);

    Math::Position* l_RobotPosition = new Math::Position(robot.x_pos, robot.y_pos, robot.angle);
    Game::Robot* l_Robot = new Robot(l_RobotPosition, robot.id);

    l_Robot->setSpeed(l_Speed);

    std::vector<Robot*> robots = m_robotsByGrid[grid_id];
    robots.push_back(l_Robot);

    return 0;
}

int RobotGame::unregisterRobot(int grid_id, int robot_id)
{
    // this is the opposite of the above
    // delete the robot from the vector it's currently in
    // question: where do we store it? why can't this be combined
    // with the register robot function?
    // cause we don't want to lose the robot, so we must store it somewhere
    // maybe we can have a map of robots with no grid
    // that register robot can look into when registering a robot to a new grid

    return 0;
}

// Handle sensor data
int RobotGame::requestSensorData(int grid_id, std::vector<int>* robot_ids)
{
    // compiles a vector of robot ids for grid with id grid_id
    // problem: what to do when a robot is looking into more than 1 grid?
    // proposed solution: send that robot id to multiple grids, which will calculate
    // the chunk of sensor data in their area, then the RobotGame will piece that info
    // together when deciding on an action

    std::vector<Robot*> robots = m_robotsByGrid[grid_id];
    std::vector<int> l_robot_ids;
    
    std::vector<Robot*>::iterator end = robots.end();

    for(std::vector<Robot*>::iterator it = robots.begin(); it != end; it++){
        l_robot_ids.push_back( (**it).m_id );
        DEBUGPRINT( "%d\n", (**it).m_id);
    }

    robot_ids = &l_robot_ids;

    return 0;
}

int RobotGame::receiveSensorData(map<int, std::vector<sensed_item> >* sensor_data)
{
    // map of sensed items for each robot id on the grid where this info is coming from
    // will be used to decide on an action, when the robot sees the complete sensor
    // there may be some trouble with pieceing more sensor data together for edge robots
    
    SensedItems::iterator iter;
    for(iter = sensor_data->begin(); iter != sensor_data->end(); iter++)
    {
        //cout << (*iter).first << " " << (*iter).second.at(0).id << endl;
        int robot_id = (*iter).first;
        Robot* l_robotp = m_robots[robot_id];
        l_robotp->updateSensors( (*iter).second );
    }

    return 0;
}

// Send and recieve actions
int RobotGame::sendAction(int grid_id, map<int, action>* robot_actions)
{
    // after a decision has been made, send it to the client
    // Shawn mentioned how the client and the grid would do this calculation twice,
    // but i don't think that's true.. the robot AI will calculate some angles and distances
    // to objects in its FOV, and then decide whether to pick up, drop or move. in the case
    // that it picks up or moves, the calculation is trivial.
    // in the case that it moves, the robot only DECIDES to move, and doesn't calculate
    // it's new position so i don't think we need to change this.

    return 0;
}

int RobotGame::actionResult(map<int, action_results>* results)
{
    // for a grid, it updates the new positions (and status) of all robots
    // question: shouldn't the action_results type have a robot

    return 0;
}
