#ifndef PUCK_H_
#define PUCK_H_

#include "GameObject.h"
#include "Position.h"

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

    Puck(Position* pos, int puck_id);


    /**
     * Returns true if the puck is held, false otherwise.
     */
    bool isHeld();

    /**
     * Toggle the status of the puck.
     */
    void toggleHeld();

    /**
     * Get X coordinate of robot.
     */
    float getX();

    /**
     * Get Y coordinate of robot.
     */
    float getY();
    
    unsigned int id;
    
private:
    /**
     * Is the puck held.
     */
    bool m_Held;
};
}

#endif
