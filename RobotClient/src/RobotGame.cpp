#include "RobotGame.h"

RobotGame::RobotGame()
{

}

RobotGame::~RobotGame()
{

}

int RobotGame::intitializeTeam(int teamid, map<int, robot_info>* robots)
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
