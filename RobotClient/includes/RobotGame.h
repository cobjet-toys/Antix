#ifndef __ROBOTGAME_H__
#define __ROBOTGAME_H__

#include <vector>
#include <map>
#include "Types.h"
#include "Robot.h"
#include "Team.h"
#include "Messages.h"

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
    int initTeam( Msg_TeamInit team);
    int setTeamRobot(int gridId, int teamId, Msg_InitRobot robot);

    // Handle sensor data
    int requestSensorData(int grid_id, vector<uid>* robot_ids);
    
	// Updates robot sensor information with newly sensed items
	// Returns -1 if sensor_data is NULL, 0 otherwise.
	int receiveSensorData(vector< pair<uid, vector<Msg_SensedObjectGroupItem> > >* sensor_data);

    // Send and recieve actions
    int sendAction(int grid_id, vector<Msg_Action>* robot_actions);
    int actionResult(vector<Msg_RobotInfo>* results);



private:

    // Map of grid ids to vector of Robot *'s
    map<int, vector<Robot*> > m_robotsByGrid;

    // Map of robot_ids to Robot *'s
    map<int, Robot*> m_robots;

    // Map of robot_ids to grid_ids
    map<uid, int> m_robotGrids;

    // Map of team ids to home coordinates
    map<int, std::pair<float, float> > m_homeLocations;

    float robot_FOV;
    float robot_Radius;
    float robot_SensorRange;
    float robot_PickupRange;
};

#endif // __ROBOTGAME_H__
