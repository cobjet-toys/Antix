#include "Game/GameObject.h"

using namespace Game;

GameObject::GameObject(Math::Position *pos):m_Position(pos)
{
}

GameObject::~GameObject()
{
    delete m_Position;
}

Math::Position* GameObject::getPosition()
{
    return m_Position;
}
