#ifndef HOME_H_
#define HOME_H_

#include "Math/Position.h"
#include "Game/GameObject.h"

namespace Game
{
/*
 * Represents the home for the robots.
 */
class Home : public GameObject
{
friend class Robotix;
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

};
}

#endif
