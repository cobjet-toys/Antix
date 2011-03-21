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

    Robot(Math::Position *pos, int teamid, unsigned int id);
    
    /**
     * Delete position object.
     */
    ~Robot();
    /**
     * Update the objects visible to the robot.
     */
    void updateSensors( std::vector<sensed_item> sensed_items );

    /**
     * Update the position of the robot.
     */
    void updatePosition();

    /**
     * Update the AI - this is what tells the robot what to do.
     */
    action getAction();
    

    /**
     * Is the robot holding a puck.
     */
    bool Holding() const;

    /**
     * Drop a puck if one is being held.
     */
    int Drop();

    /**
     * Attempt to puck up the nearest puck.
     */
    bool Pickup();

    /**
     * Attempt to puck up the nearest puck.
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


    unsigned int m_PuckHeld;    

private: 
    /**
     * Current speed of the object.
     */
    Math::Speed* m_Speed;


    /**
     * Collection of visible objects, resets at every update.
     */
    std::list< std::pair<float,float> > m_VisiblePucks;
    std::list< std::pair<float,float> > m_VisibleRobots;

    int m_TeamId;

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
