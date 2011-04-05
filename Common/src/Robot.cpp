#include "Robot.h"
#include <math.h>
#include "MathAux.h"
#include "AntixUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include "Config.h"

using namespace Game;
using namespace Antix;

float Robot::FOV;
float Robot::Radius;
float Robot::HomeRadius;
float Robot::PickupRange;
float Robot::SensorRange;
float Robot::WorldSize;


Robot::Robot(Math::Position *pos, float homeX, float homeY, unsigned int id):GameObject(pos, id)
{
    m_Speed = new Math::Speed();
    m_Position = pos;
    m_homeX = homeX;
    m_homeY = homeY;

    // Puckheld is 0 when the robot is not holding a puck
    m_PuckHeld = 0;
    m_LastPickup = new Position();
    
    time(&searchTime);
}


Robot::Robot(Math::Position *pos, unsigned int id):GameObject(pos, id)
{
    m_Speed = new Math::Speed();
    m_Position = pos;
    m_LastPickup = new Position();
}

Robot::~Robot()
{
	m_VisiblePucks.clear();
	m_VisibleRobots.clear();
    delete m_Speed;
}

int Robot::setSpeed(Speed* speed)
{
    if(speed == NULL)
    {
        return -1;
    }
    delete m_Speed;
	m_Speed = speed;
    return 0;
}


void Robot::updatePosition(const float x_pos, const float y_pos)
{
    this->getPosition()->setX(x_pos);
    this->getPosition()->setY(y_pos);
    return;
}

void Robot::updateSensors( SensedItemsList& sensedItems )
{
    // TODO Might be a more optimal way to do this, diffs?
    m_VisiblePucks.clear();
    m_VisibleRobots.clear();
    DEBUGPRINT("Updating sensor info for robot\n");

    SensedItemsList::iterator iter;
    for( iter = sensedItems.begin(); iter != sensedItems.end(); iter++)
    {
    	std::pair<unsigned int, Location> object;
    	object.first = (*iter).id;
    	object.second = Location( (*iter).x, (*iter).y );
    	
        DEBUGPRINT("Adding sensed item: %d, %d, %d\n", (*iter).id, (*iter).x, (*iter).y);
        if( (*iter).id <= 10000000 )
        {
        	
            m_VisibleRobots.push_back( object );
        }
        else
        {
			//printf("Robot has added a puck! %u\n", object.first);
            m_VisiblePucks.push_back( object );
        }
    }
}

void Robot::setPuckHeld(uid puckId)
{
    m_PuckHeld = puckId;
    return;
}

uid Robot::getPuckId()
{
    return m_PuckHeld;
}


int Robot::getAction(Msg_RobotInfo* action)
{
	if (action == NULL) return -2; // invalid input
    unsigned int toPerform = ACTION_MOVE;

    float l_HeadingError = 0.0;

    Position* l_CurrentPos = getPosition();
    float l_WorldSize = Robot::WorldSize;

    float l_x = l_CurrentPos->getX();
    float l_y = l_CurrentPos->getY();
    float l_orient = l_CurrentPos->getOrient();

    float l_Dx = Math::WrapDistance(m_homeX - l_x, Robot::WorldSize);
    float l_Dy = Math::WrapDistance(m_homeY - l_y, Robot::WorldSize);
    float l_Da = fast_atan(l_Dx, l_Dy);
    float l_Distance = hypot(l_Dx, l_Dy);
    
    //If holding a puck, drive home.
    if (Holding())
    {
    	printf("Robot ID %u is holding a freaking puck!!! ZOMG\n", m_id);
        //Turn towards home.
        l_HeadingError = Math::AngleNormalize(l_Da - l_CurrentPos->getOrient());
        
        //If we're inside the home radius, drop the puck.
        if (l_Distance < (drand48() * Robot::HomeRadius))
        {
        	printf("I'm at home\n");
            toPerform = ACTION_DROP_PUCK;
            exit(0);
        }
    }
    else
    {   
        //If we're not holding a puck.
        //If there are pucks and I'm not at home.
        
        if (m_VisiblePucks.size() > 0 && l_Distance > Robot::HomeRadius)
        {
            //Find the closest angle to the closest puck that is not being carried.
            float closestPuck = 1e9;
            for (std::vector<std::pair<unsigned int, Location> >::iterator it = m_VisiblePucks.begin();
                    it != m_VisiblePucks.end(); it++)
            {
                // If this puck is in range, try to pick it up, secondary
                // plan is to adjust course towards the nearest puck
                
                
                float puckX = it->second.first;
                float puckY = it->second.second;
                float distToPuck = hypot( puckX - l_x, puckY - l_y);
                
                if(distToPuck < Robot::PickupRange)
                {
                	//printf("Wanting to pickup puck!\n");
                    toPerform = ACTION_PICKUP_PUCK;
                    // TODO, sets this to the correct value
                    action->puckid = it->first;
                    m_LastPickup->setX(l_CurrentPos->getX());
                    m_LastPickup->setY(l_CurrentPos->getY()); 
                    break;
                }
		
				printf("distToPuck:%f closestPuck:%f\n", distToPuck, closestPuck);

                if (distToPuck < closestPuck)
                {
                    float l_Dx = Math::WrapDistance(puckX - l_x, Robot::WorldSize);
                    float l_Dy = Math::WrapDistance(puckY - l_y, Robot::WorldSize);
                    closestPuck = distToPuck;
                    l_HeadingError = Math::AngleNormalize(fast_atan(l_Dx,l_Dy) - l_orient);
                }
            }
        }
        else
        {
            //Go towards the last place a puck was found.
            float l_Dx = Math::WrapDistance(m_LastPickup->getX()-l_CurrentPos->getX(), l_WorldSize);
            float l_Dy = Math::WrapDistance(m_LastPickup->getY()-l_CurrentPos->getY(), l_WorldSize);

            l_HeadingError = Math::AngleNormalize((fast_atan(l_Dy, l_Dx) - l_CurrentPos->getOrient()));

            //If we are at the last place we found a puck, and there are no visible pucks,
            //then choose another place.
            if (hypot(l_Dx, l_Dy) < 0.05)
            {
            	//printf("nothing found at old place\n");
                m_LastPickup->setX(Math::DistanceNormalize(m_LastPickup->getX() + (drand48() * 0.4 -0.2),
                                                           l_WorldSize));
                m_LastPickup->setY(Math::DistanceNormalize(m_LastPickup->getY() + (drand48() * 0.4 -0.2),
                                                           l_WorldSize));
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
        printf("driving slow\n");
        m_Speed->setForwSpeed(0.0001);
        //Turn to reduce error.
        m_Speed->setRotSpeed(0.2 * l_HeadingError);
    }
    
    //Create action
    action->robotid = this->getId();
    
    if(toPerform == ACTION_MOVE)
    {
        action->speed = m_Speed->getForwSpeed();
        action->angle = m_Speed->getRotSpeed();
    }
    

    return toPerform; 
}

bool Robot::Holding() const
{
    if (m_PuckHeld == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

float Robot::getX()
{

	return (*getPosition()).getX();

}

float Robot::getY()
{

	return (*getPosition()).getY();

}

