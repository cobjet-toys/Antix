#include "Game/GameObject.h"
#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h> 
#endif

#include <math.h>

using namespace Game;

GameObject::GameObject(Math::Position *pos):m_Position(pos)
{
}

GameObject::~GameObject()
{
    delete m_Position;
}

Math::Position* GameObject::getPosition()
{
    return m_Position;
}

void GameObject::DrawCircle( double x, double y, double r, double count )
{
	glBegin(GL_LINE_LOOP);
	for( float a=0; a<(M_PI*2.0); a+=M_PI/count )
		glVertex2f( x + sin(a) * r, y + cos(a) * r );
	glEnd();
}

void GameObject::DrawPoint()
{
    glColor3f( 1, 1, 1);
   
    glBegin(GL_POINTS); 
    glVertex2f(getPosition()->getX(), getPosition()->getY()) ; 
    glEnd();
}
