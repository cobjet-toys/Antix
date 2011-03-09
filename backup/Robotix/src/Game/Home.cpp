#include "Game/Home.h"
using namespace Game;

float Home::m_Radius = 0.0;
float Home::m_MaxWorldSize = 0.0;

Home::Home(Math::Position* position):GameObject(position)
{ 
}

const float& Home::getRadius() const
{
    return m_Radius;
}

void Home::draw()
{
    float xloc = getPosition()->getX();
    float yloc = getPosition()->getY();
    float radius = getRadius();

    DrawCircle( xloc, yloc, radius, 16 );
	DrawCircle( xloc+m_MaxWorldSize, yloc, radius, 16 );
	DrawCircle( xloc-m_MaxWorldSize, yloc, radius, 16 );
	DrawCircle( xloc, yloc+m_MaxWorldSize, radius, 16 );
	DrawCircle( xloc, yloc-m_MaxWorldSize, radius, 16 );
}
