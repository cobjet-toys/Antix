#include "Robot.h"
#include <math.h>
#include "MathAux.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Game;

//Initialize our static variables.
float Robot::m_FOV = 0.0;
float Robot::m_Radius = 0.0;
float Robot::m_PickupRange = 0.0;
float Robot::m_SensorRange = 0.0;

Robot::Robot(Math::Position *pos,  Home* home):GameObject(pos), m_PuckHeld(NULL), m_Home(home)
{
    m_LastPickup = new Math::Position();
    m_Speed = new Math::Speed();
}

Robot::~Robot()
{
    delete m_LastPickup;
    delete m_Speed;
}

float& Robot::getRadius()
{
    return m_Radius;
}

float& Robot::getSensRange()
{
    return m_SensorRange;
}

float& Robot::getFOV()
{
    return m_FOV;
}

void Robot::updatePosition()
{
    //Calculate our displacement based on our speed and current position.
    Math::Position* l_CurrentPos = getPosition();
    float l_Dx = m_Speed->getForwSpeed() * cos(l_CurrentPos->getOrient());
    float l_Dy = m_Speed->getForwSpeed() * sin(l_CurrentPos->getOrient());
    float l_Da = m_Speed->getRotSpeed();
    float l_WorldSize = 1.0; //Robotix::getInstance()->getWorldSize();

    //Update our new position.
    l_CurrentPos->setX(Math::DistanceNormalize( l_CurrentPos->getX() + l_Dx, l_WorldSize )); 
    l_CurrentPos->setY(Math::DistanceNormalize( l_CurrentPos->getY() + l_Dy, l_WorldSize )); 
    l_CurrentPos->setOrient( Math::AngleNormalize(l_CurrentPos->getOrient() + l_Da) );

    //If we're holding a puck, update the puck position as well.
    if (false)//if (Holding())
    {
        m_PuckHeld->getPosition()->setX(l_CurrentPos->getX());
        m_PuckHeld->getPosition()->setY(l_CurrentPos->getY()); 
    }
}

void Robot::printInfo()
{

	printf("Position: %f, %f\n", (*getPosition()).getX(), (*getPosition()).getY());

}

float Robot::getX()
{

	return (*getPosition()).getX();

}

float Robot::getY()
{

	return (*getPosition()).getY();

}

/*
void draw()
{
	//to be filled
}
*/


