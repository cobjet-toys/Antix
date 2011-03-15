#include "Puck.h"

using namespace Game;

Puck::Puck(Position* pos): GameObject(pos), m_Held(false)
{
}

Puck::Puck(Position* pos, unsigned int id): GameObject(pos, id), m_Held(false)
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

