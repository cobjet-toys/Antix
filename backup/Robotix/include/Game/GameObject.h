#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include "Math/Position.h"

#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h> 
#endif

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

    virtual void draw() = 0;
    /**
     * Return the current object position.
     */
    Position* getPosition();

protected:
    void DrawCircle( double x, double y, double r, double count );
    void DrawPoint();

private:
    /**
     * Position of the current object.
     */
    Position* m_Position;
};
}
#endif
