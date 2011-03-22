#ifndef HOME_H_
#define HOME_H_

#include "Position.h"
#include "GameObject.h"

namespace Game
{
/*
 * Represents the home for the robots.
 */
class Home : public GameObject
{
public:
    /**
     * Initialize the home at location 'pos'.
     */
    Home(Math::Position* pos);

    /**
     * Return the radius of all homes.
     */
    const float& getRadius() const;

private:
    /**
     * All homes have the same radius.
     */
    static float m_Radius;

    /**
     * Maximum world size. Needs to be set before drawing using 'setWorldSize'. 
     */
    static float m_MaxWorldSize;
};
}

#endif
