#ifndef PUCK_H_
#define PUCK_H_

#include "GameObject.h"
#include "Position.h"

namespace Network
{
	class DrawServer;
}

namespace Game
{
/**
 * Puck object class.
 */
class Puck : public GameObject 
{

friend class Network::DrawServer;

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
    
    virtual void draw();
private:
    /**
     * Is the puck held.
     */
    bool m_Held;
};
}

#endif