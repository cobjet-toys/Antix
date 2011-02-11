#include "Game/Home.h"

using namespace Game;

float Home::m_Radius = 0.0;

Home::Home(Math::Position* position):GameObject(position)
{ 
}

const float& Home::getRadius() const
{
    return m_Radius;
}
