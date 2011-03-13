#ifndef ROBOT_H_
#define ROBOT_H_

#include "Speed.h"
#include "Puck.h"
#include "Home.h"
#include "Robot.h"
#include "GameObject.h"
#include "VisibleObject.h"
#include <list>

namespace Game
{

//Forward declaration required by our typedef.
class Robot;

//Typedefs that make it easier to deal with VisibleObject collections.
typedef VisibleObject<Puck*> VisiblePuckPtr;
typedef VisibleObject<Robot*> VisibleRobotPtr;

class Robot : public GameObject
{
/**
 * Individual robot class.
 */
friend class Robotix;

public:
    /**
     * Initialize robot at position 'pos' with home 'home'.
     */
    Robot(Math::Position *pos, Home* home);
    
    Robot(Math::Position *pos);
    
    /**
     * Delete position object.
     */
    ~Robot();
    /**
     * Update the objects visible to the robot.
     */
    void updateSensors();

    /**
     * Update the position of the robot.
     */
    void updatePosition();

    /**
     * Update the AI - this is what tells the robot what to do.
     */
    void updateController();

    /**
     * Is the robot holding a puck.
     */
    bool Holding() const;

    /**
     * Drop a puck if one is being held.
     */
    bool Drop();

    /**
     * Attempt to puck up the nearest puck.
     */
    bool Pickup();

    /**
     * Print data about the robot.
     */
    void printInfo();

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

    /*
     * ID of the robot
     */
    int id;


private: 
    /**
     * Current speed of the object.
     */
    Math::Speed* m_Speed;

    /**
     * The puck being held, if any.
     */
    Puck* m_PuckHeld;

    /**
     * Home of the robot.
     */
    Home* m_Home;

    /**
     * The last position a puck was found at.
     */
    Math::Position* m_LastPickup;

    /**
     * Collection of visible objects, resets at every update.
     */
    std::list<VisiblePuckPtr> m_VisiblePucks;
    std::list<VisibleRobotPtr> m_VisibleRobots;

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
