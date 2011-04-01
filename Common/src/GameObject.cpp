#include "GameObject.h"
#include <math.h>
#include <stdio.h>
#include "Config.h"

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

	DEBUGPRINT("GAMEOBJECT STATUS:\t ID:%d Xpos:%f, Ypos:%f\n",  m_id, (*getPosition()).getX(), (*getPosition()).getY());

}

void GameObject::setPosition(const float x_pos, const float y_pos, const float orient)
{
    m_Position->setX(x_pos);
    m_Position->setY(y_pos);
    m_Position->setOrient(orient);
    return;
}


float GameObject::getX()
{

	return (*getPosition()).getX();

}

float GameObject::getY()
{

	return (*getPosition()).getY();

}

int GameObject::getId()
{

	return m_id;
}
