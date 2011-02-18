#ifndef PUCK_H_
#define PUCK_H_

#include "Game/GameObject.h"
#include "Math/Position.h"

namespace Game
{
/**
 * Puck object class.
 */
class Puck : public GameObject 
{
public:
    /**
     * Initialize the puck at location 'pos', and held to false.
     */
    Puck(Position* pos);

    /**
     * Returns true if the puck is held, false otherwise.
     */
    bool isHeld();

    /**
     * Toggle the status of the puck.
     */
    void toggleHeld();

    /**
     * Get X coordinate of puck.
     */
    float getX();

    /**
     * Get Y coordinate of puck.
     */
    float getY();
private:
    /**
     * Is the puck held.
     */
    bool m_Held;

    
};
}

#endif
