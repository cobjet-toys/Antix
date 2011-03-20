#ifndef __ROBOTGAME_H__
#define __ROBOTGAME_H__

#include <vector>
#include <map>
#include "Types.h"
#include "Robot.h"
#include "Team.h"

using std::map;
using std::vector;
using std::pair;

using namespace Game;

class RobotGame
{
public:

    RobotGame();
    ~RobotGame();


    // Initialization functions
    //int intitializeTeam(int grid_id, vector<int> team_mapping);
    //int receiveInitialRobots(int grid_id, vector<robot_info> robot_info_vector);

    // New Initialization functions
    int initTeam( int id, float x, float y);
    int setTeamRobot(int teamId, int robotId, float x, float y);

    // Register and UnRegister robots from a particular grid
    int registerRobot(int grid_id, robot_info robot);
    int unregisterRobot(int grid_id, int robot_id);

    // Handle sensor data
    int requestSensorData(int grid_id, vector<int>* robot_ids);
    int receiveSensorData(map<int, vector<sensed_item> >* sensor_data);

    // Send and recieve actions
    int sendAction(int grid_id, map<int, action>* robot_actions);
    int actionResult(map<int, action_results>* results);

private:

    // Map of grid ids to Robot *'s
    map<int, vector<Robot*> > m_robotsInGrid;

    // Map of robot_ids to Robot *'s
    map<int, Robot*> m_robots;

    // Map of team ids to home coordinates
    map<int, std::pair<float, float> > m_homeLocations;

    float robot_FOV;
    float robot_Radius;
    float robot_SensorRange;
    float robot_PickupRange;
};

#endif //
