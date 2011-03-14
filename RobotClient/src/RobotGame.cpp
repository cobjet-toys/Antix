#include "RobotGame.h"
#include "Team.h"

RobotGame::RobotGame()
{
    // instantiate team objects with range of robots
    // the teams number is given in the config

}

RobotGame::~RobotGame()
{

}

// Compile grid to team mapping
int RobotGame::intitializeTeam(map<int, vector<int> >* team_mapping)
{

    return 0;
}

// Register and UnRegister robots from a particular grid
int RobotGame::registerRobot(int fd, robot_info robot)
{

    return 0;
}

int RobotGame::unregisterRobot(int fd, int robot_id)
{

    return 0;
}

// Handle sensor data
int RobotGame::requestSensorData(int fd, vector<int>* robot_id)
{

    return 0;
}

int RobotGame::receiveSensorData(map<int, vector<sensed_item> >* sensor_data)
{

    return 0;
}

// Send and recieve actions
int RobotGame::sendAction(int fd, map<int, action>* robot_actions)
{

    return 0;
}

int RobotGame::actionResult(map<int, action_results>* results)
{

    return 0;
}
