#include "Robot.h"
#include <math.h>
#include "MathAux.h"
#include "AntixUtil.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Game;
using namespace Antix;

Robot::Robot(Math::Position *pos, int teamid, unsigned int id):GameObject(pos, id), m_PuckHeld(NULL)
{
    m_Speed = new Math::Speed();

    m_TeamId = teamid;

    //TODO: get TEAM_SIZE from the config
    //int TEAM_SIZE = 1000;
    //m_Team = id/TEAM_SIZE;
    
    m_PuckHeld = -1;

}


Robot::Robot(Math::Position *pos, unsigned int id):GameObject(pos, id), m_PuckHeld(NULL)
{
    m_Speed = new Math::Speed();

    //TODO: get TEAM_SIZE from the config
    //int TEAM_SIZE = 1000;
    //m_Team = id/TEAM_SIZE;
    
    m_PuckHeld = -1;

}

Robot::~Robot()
{
    delete m_Speed;
}

int Robot::setSpeed(Speed* speed)
{

    return 0;

}


void Robot::updatePosition()
{
    /*
    //Calculate our displacement based on our speed and current position.
    Math::Position* l_CurrentPos = getPosition();
    float l_Dx = m_Speed->getForwSpeed() * cos(l_CurrentPos->getOrient());
    float l_Dy = m_Speed->getForwSpeed() * sin(l_CurrentPos->getOrient());
    float l_Da = m_Speed->getRotSpeed();
    float l_WorldSize = Robotix::getInstance()->getWorldSize();

    //Update our new position.
    l_CurrentPos->setX(Math::DistanceNormalize( l_CurrentPos->getX() + l_Dx, l_WorldSize )); 
    l_CurrentPos->setY(Math::DistanceNormalize( l_CurrentPos->getY() + l_Dy, l_WorldSize )); 
    l_CurrentPos->setOrient( Math::AngleNormalize(l_CurrentPos->getOrient() + l_Da) );

    //If we're holding a puck, update the puck position as well.
    if (Holding())
    {
        m_PuckHeld->getPosition()->setX(l_CurrentPos->getX());
        m_PuckHeld->getPosition()->setY(l_CurrentPos->getY()); 
    }
    */
}

void Robot::updateSensors( std::vector<sensed_item> sensed_items )
{
    // TODO Might be a more optimal way to do this, diffs?
    m_VisiblePucks.clear();
    m_VisibleRobots.clear();

    std::vector<sensed_item>::iterator iter;
    for( iter = sensed_items.begin(); iter != sensed_items.end(); iter++)
    {
        if( getType((*iter).id) == ROBOT )
        {
            m_VisibleRobots.push_back( Location( (*iter).x, (*iter).y ) );
        }
        else
        {
            m_VisiblePucks.push_back( Location( (*iter).x, (*iter).y ) );
        }
    }

/*
    //Clear our collection of visible objets.
    m_VisiblePucks.clear();
    m_VisibleRobots.clear();


    Position* l_CurrentPos = getPosition();
    float l_WorldSize = Robotix::getInstance()->getWorldSize();

    //Note: The following two large sensing operations could safely be done in parallel since
    //they do not modify any common data.
    
    //Check every robot in the world, and determine if it is in range.
    //If it is, push it into our visible robots collection. 
    for (RobotIter it = Robotix::getInstance()->getFirstRobot();
            it != Robotix::getInstance()->getLastRobot(); it++)
    {
        Robot *l_Other = *it;
        
        //Discard if it's the same robot.
        if (l_Other == this)
            continue;

        Position* l_OtherPos = l_Other->getPosition();
        
        //Discard if it's out of range. Hypot computation we leave for last, as it's the most
        //expensive.
        float l_Dx = Math::WrapDistance(l_OtherPos->getX() - l_CurrentPos->getX(), l_WorldSize );
        if (fabs(l_Dx) > m_SensorRange)
            continue;

        float l_Dy = Math::WrapDistance(l_OtherPos->getY() - l_CurrentPos->getY(), l_WorldSize );
        if (fabs(l_Dy) > m_SensorRange)
            continue;

        float l_Range = hypot (l_Dx, l_Dy);

        if ( l_Range > m_SensorRange )
            continue;

        //Discard if it's out of our FOV.
        float l_AbsHeading = atan2 (l_Dy, l_Dx);
        float l_RelHeading = Math::AngleNormalize(l_AbsHeading - l_CurrentPos->getOrient());
        if (fabs(l_RelHeading) > m_FOV/2.0)
            continue;

        //The robot is in our range and FOV, add it to visible robots.
        m_VisibleRobots.push_back(VisibleRobotPtr(l_Other, l_Range, l_RelHeading));
    }

    //Check every puck in the world, and determine if it's in range.
    //If it is, push it into our visible puck collection.
    for (PuckIter it = Robotix::getInstance()->getFirstPuck();
            it != Robotix::getInstance()->getLastPuck(); it++)
    {
        Puck* l_Puck = (*it);
        Position* l_PuckPosition = l_Puck->getPosition();

        //Discard if it's out of range. Hypot computation we leave for last, as it's the most
        //expensive.
        float l_Dx = Math::WrapDistance(l_PuckPosition->getX() - l_CurrentPos->getX(), l_WorldSize );
        if (fabs(l_Dx) > m_SensorRange)
            continue;

        float l_Dy = Math::WrapDistance(l_PuckPosition->getY() - l_CurrentPos->getY(), l_WorldSize );
        if (fabs(l_Dy) > m_SensorRange)
            continue;

        float l_Range = hypot (l_Dx, l_Dy);
        if (l_Range > m_SensorRange)
            continue;

        //Discard if it's out of our FOV.
        float l_AbsHeading = atan2 (l_Dy, l_Dx);
        float l_RelHeading = Math::AngleNormalize(l_AbsHeading - l_CurrentPos->getOrient());
        if (fabs(l_RelHeading) > m_FOV/2.0)
            continue;

        //The puck is in our range and FOV, add it to visible pucks.
        m_VisiblePucks.push_back(VisiblePuckPtr(l_Puck, l_Range, l_RelHeading));
    }
     */
}

void Robot::updateController()
{
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
    if (m_PuckHeld == -1)
    {
        return true;
    }
    else{
        return false;
    }
}

int Robot::Drop()
{
    unsigned int temppuck;

    //If we're holding a puck, drop it.
    if (Holding())
    {
        m_PuckHeld = NULL;
    }
    return temppuck;
}

bool Robot::Pickup()
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
    return false;
}

float Robot::getX()
{

	return (*getPosition()).getX();

}

float Robot::getY()
{

	return (*getPosition()).getY();

}


