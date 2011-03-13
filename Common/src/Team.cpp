#include "Team.h"

using namespace Game;


Team::Team()
{

    /* TODO Fill in logically
    //Position a random home location;
    Math::Position* l_HomePosition = Math::Position::randomPosition(Robotix::getInstance()->getWorldSize()); 
    m_Home = new Home( l_HomePosition );
    
    //Generate our robots.
    for (unsigned int i = 0; i < m_RobotPopCount; i++)
    {
       //Get a random location.
       Math::Position* l_RobotPosition = Math::Position::randomPosition(Robotix::getInstance()->getWorldSize());
       
       //Create our robot
       Robot* l_Robot = new Robot(l_RobotPosition, m_Home);
       
       //Add robot to our team.
       m_Robots.push_back( l_Robot );

       //Add robot to our global population.
       Robotix::getInstance()->addRobotToPop(l_Robot); 
    }

    //Generate a random team color;
    m_Color = GUI::Color::getRandomColor();
    */
}

Team::~Team()
{
/*
    //Delete our robots.
    for (std::list<Robot*>::iterator it = m_Robots.begin(); it != m_Robots.end(); it++)
    {
        delete (*it);
    }
    delete m_Color;
    delete m_Home;
*/
}

Home* Team::getHome()
{
    return m_Home;
}

