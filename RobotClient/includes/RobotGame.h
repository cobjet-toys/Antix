#ifndef __ROBOTGAME_H__
#define __ROBOTGAME_H__

#include <vector>
#include <map>
#include "Types.h"
#include "Robot.h"

using std::map;
using std::vector;

using namespace Game;

class RobotGame
{
public:

    RobotGame();
    ~RobotGame();

    // Compile grid to team mapping
    int intitializeTeam(map<int, vector<int> >* team_mapping);

    // Register and UnRegister robots from a particular grid
    int registerRobot(int fd, robot_info robot);
    int unregisterRobot(int fd, int robot_id);

    // Handle sensor data
    int requestSensorData(int fd, vector<int>* robot_id);
    int receiveSensorData(map<int, vector<sensed_item> >* sensor_data);

    // Send and recieve actions
    int sendAction(int fd, map<int, action>* robot_actions);
    int actionResult(map<int, action_results>* results);

private:

    //Map of file descriptors to Grids 
    //map<int, int> m_gridMap;

    //Map of grid ids to Robot *'s
    map<int, vector<Robot*> > m_Robots;

};

#endif //
