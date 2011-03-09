#include "GUI/Color.h"
#include <stdlib.h>

using namespace GUI;

Color::Color(const float& r, const float& g, const float& b): m_R(r), m_G(g), m_B(b)
{
}

Color* Color::getRandomColor()
{
   Color* l_Color = new Color(drand48(), drand48(), drand48()); 
   return l_Color;
}

const float& Color::getR() const
{
    return m_R;
}

const float& Color::getG() const
{
    return m_G;
}

const float& Color::getB() const
{
    return m_B;
}
