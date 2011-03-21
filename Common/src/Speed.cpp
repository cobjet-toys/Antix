#include "Speed.h"

using namespace Math;

Speed::Speed():m_ForwSpeed(0.0), m_RotSpeed(0.0)
{
}

Speed::Speed(const float fwSpeed, const float rotSpeed):m_ForwSpeed(fwSpeed), m_RotSpeed(rotSpeed)
{
}

void Speed::setRotSpeed(const float& rot)
{
    m_RotSpeed = rot;
}

void Speed::setForwSpeed(const float& forw) 
{
    m_ForwSpeed = forw;
}
const float& Speed::getForwSpeed() const
{
    return m_ForwSpeed;
}

const float& Speed::getRotSpeed() const
{
    return m_RotSpeed;
}
