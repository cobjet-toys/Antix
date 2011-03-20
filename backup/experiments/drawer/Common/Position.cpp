#include "Position.h"
#include <stdlib.h>

using namespace Math;

Position::Position():m_Xpos(0.0), m_Ypos(0.0), m_Orient(0.0)
{
}

Position::Position(const float& xpos, const float& ypos, const float& orient)
    :m_Xpos(xpos), m_Ypos(ypos), m_Orient(orient)
{
}

const float& Position::getX() const
{
    return m_Xpos;
}

const float& Position::getY() const
{
    return m_Ypos;
}

const float& Position::getOrient() const
{
    return m_Orient;
}

void Position::setOrient(const float& orient)
{
    m_Orient = orient;
}
   
void Position::setX(const float& xloc)
{
    m_Xpos = xloc;
}

void Position::setY(const float& yloc)
{
    m_Ypos = yloc;
}

Position* Position::randomPosition(const float& maxSize)
{ 
    Position *newPos = new Position(drand48()*maxSize, drand48()*maxSize, 0.0f);
    return newPos;
}

