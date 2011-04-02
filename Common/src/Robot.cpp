#include "Robot.h"
#include <math.h>
#include "MathAux.h"
#include "AntixUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include "Config.h"

using namespace Game;
using namespace Antix;

Robot::Robot(Math::Position *pos, float homeX, float homeY, unsigned int id):GameObject(pos, id)
{
    m_Speed = new Math::Speed();
    m_homeX = homeX;
    m_homeY = homeY;

    // Puckheld is 0 when the robot is not holding a puck
    m_PuckHeld = 0;
}


Robot::Robot(Math::Position *pos, unsigned int id):GameObject(pos, id)
{
    m_Speed = new Math::Speed();
}

Robot::~Robot()
{
	m_VisiblePucks.clear();
	m_VisibleRobots.clear();
    delete m_Speed;
}

Msg_Action Robot::setSpeed(Speed* speed)
{
    /*
    if(speed == NULL)
    {
        return -1;
    }
    delete m_Speed;
	m_Speed = speed;
    return 0;*/
    Msg_Action l_action;
    return l_action;
}


void Robot::updatePosition(const float x_pos, const float y_pos)
{
    this->getPosition()->setX(x_pos);
    this->getPosition()->setY(y_pos);
    return;
}

void Robot::updateSensors( SensedItemsList sensedItems )
{
    // TODO Might be a more optimal way to do this, diffs?
    m_VisiblePucks.clear();
    m_VisibleRobots.clear();
    DEBUGPRINT("Updating sensor info for robot\n");

    SensedItemsList::iterator iter;
    for( iter = sensedItems.begin(); iter != sensedItems.end(); iter++)
    {
        DEBUGPRINT("Adding sensed item: %d, %d, %d\n", (*iter).id, (*iter).x, (*iter).y);
        if( getType((*iter).id) == ROBOT )
        {
            m_VisibleRobots.push_back( Location( (*iter).x, (*iter).y ) );
        }
        else
        {
            m_VisiblePucks.push_back( Location( (*iter).x, (*iter).y ) );
        }
    }
}

void Robot::setPuckHeld(uid puckId)
{
    m_PuckHeld = puckId;
    return;
}

Msg_Action Robot::getAction()
{
    Msg_Action l_action;
    l_action.robotid = this->getId();
    //l_action.action = 1;
    //l_action.speed = 1.0;
    //l_action.angle = 0.0f;
    //return l_action;

    /*
    float l_HeadingError = 0.0;

    Position* l_CurrentPos = getPosition();
    Position* l_HomePos = m_Home->getPosition();
    float l_WorldSize = Robotix::getInstance()->getWorldSize();

    //Distance and angle to home.
    float l_Dx = Math::WrapDistance(l_HomePos->getX() - l_CurrentPos->getX(), l_WorldSize);
    float l_Dy = Math::WrapDistance(l_HomePos->getY() - l_CurrentPos->getY(), l_WorldSize);
    float l_Da = atan2(l_Dy, l_Dx);
    float l_Distance = hypot(l_Dx, l_Dy);

    //If holding a puck, drive home.
    if (Holding())
    {
        //Turn towards home.
        l_HeadingError = Math::AngleNormalize(l_Da - l_CurrentPos->getOrient());
        
        //If we're inside the home radius, drop the puck.
        if (l_Distance < (drand48() * m_Home->getRadius()))
            Drop();
    }
    //If we're not holding a puck.
    else
    {
        //If there are pucks and I'm not at home.
        if (m_VisiblePucks.size() > 0 && l_Distance > m_Home->getRadius())
        {
            //Find the closest angle to the closest puck that is not being carried.
            float l_ClosestRange = 1e9;
            for (std::list<VisiblePuckPtr>::iterator it = m_VisiblePucks.begin();
                    it != m_VisiblePucks.end(); it++)
            {
                VisiblePuckPtr l_Puck = *it;
                if (l_Puck.getRange() < l_ClosestRange && !l_Puck.getObject()->isHeld())
                {
                    l_HeadingError = l_Puck.getOrient();
                    l_ClosestRange = l_Puck.getRange();
                }
            }

            //Attempt to puck up the puck.
            if (Pickup())
            {
                //Got one! Remember where it was.
                m_LastPickup->setX(l_CurrentPos->getX());
                m_LastPickup->setY(l_CurrentPos->getY()); 
            }
        }
        else
        {
            //Go towards the last place a puck was found.
            float l_Dx = Math::WrapDistance(m_LastPickup->getX()-l_CurrentPos->getX(), l_WorldSize);
            float l_Dy = Math::WrapDistance(m_LastPickup->getY()-l_CurrentPos->getY(), l_WorldSize);

            l_HeadingError = Math::AngleNormalize((atan2(l_Dy, l_Dx) - l_CurrentPos->getOrient()));

            //If we are at the last place we found a puck, and there are no visible pucks,
            //then choose another place.
            if (hypot(l_Dx, l_Dy) < 0.05)
            {
                m_LastPickup->setX(m_LastPickup->getX() + (drand48() * 0.4 -0.2));
                m_LastPickup->setY(m_LastPickup->getY() + (drand48() * 0.4 -0.2));
                
                m_LastPickup->setX(Math::DistanceNormalize(m_LastPickup->getX(), l_WorldSize));
                m_LastPickup->setY(Math::DistanceNormalize(m_LastPickup->getY(), l_WorldSize));
            }

        }
    }
    
    //If I'm pointing in the right direction:
    if (fabs(l_HeadingError) < 0.1)
    {
        //Drive fast!
        m_Speed->setForwSpeed(0.005);
        //Don't turn!
        m_Speed->setRotSpeed(0.0);
    }
    else
    {
        //Drive slow.
        m_Speed->setForwSpeed(0.001);
        //Turn to reduce error.
        m_Speed->setRotSpeed(0.2 * l_HeadingError);
    }
    */
}

bool Robot::Holding() const
{
    if (m_PuckHeld == 0)
    {
        return true;
    }
    else{
        return false;
    }
}

Msg_Action Robot::Drop()
{
    /*unsigned int temppuck;

    //If we're holding a puck, drop it.
    if (Holding())
    {
        m_PuckHeld = NULL;
    }
    return temppuck; */
    Msg_Action l_action;
    return l_action;
}

Msg_Action Robot::Pickup()
{
    /*
    // TODO: Not deleting this yet, may be needed for actions
    if (!Holding())
    {
        //Check our list of visible pucks.
        for (std::list<VisiblePuckPtr>::iterator it = m_VisiblePucks.begin();
                it != m_VisiblePucks.end(); it++)
        {
            //Puck up the first puck in our range that isn't held.
            Puck* l_Puck = (*it).getObject();
            if ((*it).getRange() < m_PickupRange && !l_Puck->isHeld())
            {
                m_PuckHeld = l_Puck;
                m_PuckHeld->toggleHeld();
                return true;
            }
        }
    }
    */
    Msg_Action l_action;
    return l_action;
}

float Robot::getX()
{

	return (*getPosition()).getX();

}

float Robot::getY()
{

	return (*getPosition()).getY();

}

