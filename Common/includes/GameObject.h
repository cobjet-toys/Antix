#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include "Position.h"


using namespace Math;

namespace Game
{
/**
 * Represents a generic object on our world.
 */
class GameObject
{
public:
    /**
     * Takes a pointer to a dynamically allovated position object.
     */
    GameObject(Position *pos);
    GameObject(Position *pos, unsigned int m_id);

    /**
     * Delete the position.
     */
    virtual ~GameObject();

    /**
     * Return the current object position.
     */
    Position* getPosition();

    unsigned int m_id;

    /**
     * Print data about the robot.
     */
    void printInfo();

    /**
     * Get X coordinate of object.
     */
    float getX();

    /**
     * Get Y coordinate of object.
     */
    float getY();

    /**
     * Get id of object.
     */
    int getId();

    void setPosition(const float x_pos, const float y_pos, const float orient);


protected:
    /**
     * Position of the current object.
     */
    Position* m_Position;
    

};
}
#endif
