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
    void updateSensors(SensedItemsList sensedItems );

    /**
     * Update the position of the robot.
     */
    void updatePosition(const float x_pos, const float y_pos);

    /**
     * Change the ID of the puck that is currently held
     */
     void setPuckHeld(unsigned int puckId);

    /**
     * Update the AI - this is what tells the robot what to do.
     */
    Msg_Action getAction();

    /**
     * Is the robot holding a puck.
     */
    bool Holding() const;

    /**
     * Drop a puck if one is being held.
     */
    Msg_Action Drop();

    /**
     * Attempt to puck up the nearest puck.
     */
    Msg_Action Pickup();

    /**
     * Attempt to puck up the nearest puck.
     */
    Msg_Action setSpeed(Speed* speed);


    /**
     * Get X coordinate of robot.
     */
    float getX();

    /**
     * Get Y coordinate of robot.
     */
    float getY();

    /**
     * Return the radius of the robot.
     */
    static float& getRadius();

    /**
     * Return the sensor range of all robots.
     */

    static float& getSensRange();

    /**
     * Return the FOV of all robots.
     */

    static float& getFOV();



private: 
    /**
     * Current speed of the object.
     */
    Math::Speed* m_Speed;

    /**
     * A robot id if this robot is holding a puck, 0 otherwise.
     */
    unsigned int m_PuckHeld;    

    /**
     * Collection of visible objects, resets at every update.
     */
    std::list< std::pair<float,float> > m_VisiblePucks;
    std::list< std::pair<float,float> > m_VisibleRobots;

    /*
     * Absolute location of this robot's home
     */
    float m_homeX;
    float m_homeY;
    
    /**
     * FOV of any robot.
     */
    static float m_FOV;

    /**
     * Radius of any robot.
     */
    static float m_Radius;

    /**
     * Range at which a robot can pick a puck up at.
     */
    static float m_PickupRange;

    /**
     * Range at which the robot can see other objects.
     */
    static float m_SensorRange;
};
}


#endif
