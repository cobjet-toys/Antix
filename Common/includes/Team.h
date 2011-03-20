#ifndef TEAM_H_
#define TEAM_H_

#include "Speed.h"
#include "Puck.h"
#include "Home.h"
#include "Team.h"
#include "GameObject.h"
#include "VisibleObject.h"
#include <list>

namespace Game
{

class Team : public GameObject
{

public:
    
    Team(Math::Position *pos, unsigned int id);
    
    /**
     * Delete position object.
     */
    ~Team();

    float getX();
    float getY();

private: 

};
}

#endif
