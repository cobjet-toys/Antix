#ifndef ROBOT_H_
#define ROBOT_H_

#include "Speed.h"
#include "Puck.h"
#include "Robot.h"
#include "Team.h"
#include "GameObject.h"
#include "VisibleObject.h"
#include "Types.h"
#include <list>
#include <vector>
#include <time.h>

namespace Game
{

//Forward declaration required by our typedef.
class Robot;

//Typedefs that make it easier to deal with VisibleObject collections.
typedef VisibleObject<Puck*> VisiblePuckPtr;
typedef VisibleObject<Robot*> VisibleRobotPtr;
typedef std::vector<Robot*> RobotList;

class Robot : public GameObject
{

public:

    Robot(Math::Position *pos, unsigned int id);

    Robot(Math::Position *pos, float homeX, float homeY, unsigned int id);
    
    /**
     * Delete position object.
     */
    ~Robot();
    /**
     * Update the objects visible to the robot.
     */
    void updateSensors(SensedItemsList& sensedItems );

    /**
     * Update the position of the robot.
     */
    void updatePosition(const float x_pos, const float y_pos);

    /**
     * Update the AI - this is what tells the robot what to do.
     */
    int getAction(Msg_RobotInfo *action);

    /**
     * Is the robot holding a puck.
     */
    bool Holding() const;

    /**
     * Set the speed of this robot.
     */
    int setSpeed(Speed* speed);


    /**
     * Get X coordinate of robot.
     */
    float getX();

    /**
     * Get Y coordinate of robot.
     */
    float getY();

    /**
     * Make this robot hold on to a new puck
     */
    void setPuckHeld(uid puckToHold);

    /**
     * Returns the id of the puck being held, 0 if it is not holding a puck
     */
    uid getPuckId();

    /**
     * FOV of any robot.
     */
    static float FOV;

    /**
     * Radius of any robot.
     */
    static float Radius;

    /**
     * Size of any home
     */
     static float HomeRadius;

    /**
     * Range at which a robot can pick a puck up at.
     */
    static float PickupRange;

    /**
     * Range at which the robot can see other objects.
     */
    static float SensorRange;

    /**
     * Size of the world
     */
     static float WorldSize;

private: 

    time_t searchTime;

    /**
     * Current speed of the object.
     */
     
     
    Math::Speed* m_Speed;

    /**
     * Current position of the robot
     */
    Math::Position* m_Position;

    /**
     * A robot id if this robot is holding a puck, 0 otherwise.
     */
    unsigned int m_PuckHeld;    

    /**
     * Collection of visible objects, resets at every update.
     */
    //ObjectLocationList m_VisiblePucks;
    //ObjectLocationList m_VisibleRobots;
    
    std::vector<std::pair<unsigned int, Location> > m_VisiblePucks;
    std::vector<std::pair<unsigned int, Location> > m_VisibleRobots;

    /*
     * Absolute location of this robot's home
     */
    float m_homeX;
    float m_homeY;

    /*
     * Location of last picked up puck
     */
     Math::Position* m_LastPickup;
    
};

} //end namespace Game


#endif
