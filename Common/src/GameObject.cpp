#include "GameObject.h"
#include <math.h>
#include <stdio.h>

using namespace Game;

GameObject::GameObject(Math::Position *pos):m_Position(pos)
{
}

GameObject::GameObject(Math::Position *pos, unsigned int id):m_Position(pos)
{
    m_id = id;
}

GameObject::~GameObject()
{
    delete m_Position;
}

Math::Position* GameObject::getPosition()
{
    return m_Position;
}

void GameObject::printInfo()
{

	printf("Position: %f, %f - id:%d\n", (*getPosition()).getX(), (*getPosition()).getY(), m_id);

}

float GameObject::getX()
{

	return (*getPosition()).getX();

}

float GameObject::getY()
{

	return (*getPosition()).getY();

}
