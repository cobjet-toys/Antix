#include "Game/Puck.h"

using namespace Game;

Puck::Puck(Position* pos): GameObject(pos), m_Held(false)
{
}

bool Puck::isHeld()
{
    return m_Held == true;
}

void Puck::toggleHeld()
{
    m_Held = !m_Held;
}

float Puck::getX()
{

	return (*getPosition()).getX();

}

float Puck::getY()
{

	return (*getPosition()).getY();

}
