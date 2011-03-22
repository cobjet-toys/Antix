#ifndef TEAM_H_
#define TEAM_H_

#include "Speed.h"
#include "Puck.h"
#include "Home.h"
#include "Team.h"
#include "GameObject.h"
#include "VisibleObject.h"
#include <list>

namespace Network
{
    class DrawServer;
}

namespace Game
{

class Team : public GameObject
{
friend class Network::DrawServer;

public:
    /**
     * Ctor. Initialize with random color, and home at random position.
     */
    Team();
public:
    
    Team(Math::Position *pos, unsigned int id);
    
    /**
     * Delete position object.
     */
    ~Team();

    float getX();
    float getY();

    int m_TeamId;

private: 

};
}

#endif
