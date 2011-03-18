#include "Math/Position.h"

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

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

    /**
     * Delete the position.
     */
    virtual ~GameObject();

    /**
     * Return the current object position.
     */
    Position* getPosition();
private:
    /**
     * Position of the current object.
     */
    Position* m_Position;
};
}
#endif
