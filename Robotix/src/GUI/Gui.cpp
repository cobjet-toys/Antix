#include "GUI/Gui.h"
#include "Game/Game.h"

#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h> 
#endif

#include <math.h>
#include "Math/Math.h"

void idleFunc()
{
    Game::Robotix::getInstance()->update();
}

void timerFunc(int dummy)
{
    glutPostRedisplay();
}

void displayFunc()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();    
    
    //Draw the pucks
    for (Game::PuckIter it = Game::Robotix::getInstance()->getFirstPuck();
            it != Game::Robotix::getInstance()->getLastPuck(); it++)
    {
        (*it)->draw();
    }
    
    for (std::list<Game::Team*>::iterator it = Game::Robotix::getInstance()->getFirstTeam();
            it != Game::Robotix::getInstance()->getLastTeam();it++)
    {
        (*it)->getHome()->draw();
        
                for (std::list<Game::Robot*>::iterator rt = (*it)->getFirstRobot();
	    		rt != (*it)->getLastRobot(); rt++)
	    {
            (*rt)->draw();
	    }
    }

    glutSwapBuffers();
    glutTimerFunc( 20, timerFunc, 0);
}

void drawPucks()
{
    glColor3f( 1, 1, 1);
   
    glBegin(GL_POINTS); 
    
    for (Game::PuckIter it = Game::Robotix::getInstance()->getFirstPuck();
            it != Game::Robotix::getInstance()->getLastPuck(); it++)
    {
        glVertex2f( (*it)->getPosition()->getX(), (*it)->getPosition()->getY() );
    }
    glEnd();
   
}

void drawTeams()
{
    for (std::list<Game::Team*>::iterator it = Game::Robotix::getInstance()->getFirstTeam();
            it != Game::Robotix::getInstance()->getLastTeam();it++)
    {
        const GUI::Color *l_TeamColor = (*it)->getColor();
        drawHome((*it)->getHome(), l_TeamColor);
        
        glColor3f(l_TeamColor->getR(), l_TeamColor->getG(), l_TeamColor->getB());
	    glBegin(GL_POINTS);
        for (std::list<Game::Robot*>::iterator rt = (*it)->getFirstRobot();
	    		rt != (*it)->getLastRobot(); rt++)
	    {
             glVertex2f( (*rt)->getPosition()->getX(), (*rt)->getPosition()->getY() );
	    	//drawRobot((*rt), (*it)->getColor());
	    }
        glEnd();
    }
}

void drawRobot(Game::Robot* robot, const GUI::Color* color )
{
    glPushMatrix();
        Position* lPos = robot->getPosition();

        glTranslatef(lPos->getX(), lPos->getY(), 0.0);
        glRotatef(Math::rtod(lPos->getOrient()), 0, 0, 1);

	glColor3f(color->getR(), color->getG(), color->getB());

	float lRadius = Game::Robot::getRadius();
	float lWinSize = Game::Robotix::getInstance()->getWindowSize();
	float lWorldSize = Game::Robotix::getInstance()->getWorldSize();

	if (lRadius * lWinSize / lWorldSize < 2.0)
	{
	    glBegin(GL_POINTS);
	    glVertex2f(0,0);
	    glEnd();
	}
	else
	{
        //Draw the robot body.
	    glBegin(GL_LINE_LOOP);
	    for (float a = 0.0; a < (M_PI*2.0); a+=M_PI/16)
		    glVertex2f( sin(a) * lRadius, cos(a) * lRadius);
	    glEnd();
        //Draw the robot orientation.
	    glBegin(GL_LINES);
	    glVertex2f(0,0);
        glVertex2f(Game::Robot::getRadius(), 0);
	    glEnd();

        //Draw the robot FOV:
/*        glBegin(GL_LINE_LOOP);
        glVertex2f(0,0);
        float l_FOV = Game::Robot::getFOV();
        float l_Right = -l_FOV/2.0;
        float l_Left = +l_FOV/2.0;
        float l_Incr = l_FOV/32.0;

        float l_SensRange = Game::Robot::getSensRange();
        for(float i = l_Right; i < l_Left; i+=l_Incr)
            glVertex2f(cos(i)* l_SensRange, sin(i)*l_SensRange);
        glVertex2f(cos(l_Left)* l_SensRange, sin(l_Left)*l_SensRange);
        glEnd();
*/
        /*
        //Draw bounding box.
        glBegin(GL_LINE_LOOP);
        glVertex2f(0.0, sin(l_Left)*l_SensRange);
        glVertex2f(l_SensRange, sin(l_Left)*l_SensRange);
        glVertex2f(l_SensRange, sin(l_Right)*l_SensRange);
        glVertex2f(0.0, sin(l_Right)*l_SensRange);
        glEnd();*/
/*
        glBegin(GL_LINE_LOOP);
        glVertex2f(-l_SensRange, -l_SensRange);
        glVertex2f(-l_SensRange, l_SensRange);
        glVertex2f(l_SensRange, l_SensRange);  
        glVertex2f(l_SensRange, -l_SensRange);
        glEnd();*/
    }

    glPopMatrix();
}

// utility
void GlDrawCircle( double x, double y, double r, double count )
{
	glBegin(GL_LINE_LOOP);
	for( float a=0; a<(M_PI*2.0); a+=M_PI/count )
		glVertex2f( x + sin(a) * r, y + cos(a) * r );
	glEnd();
}


void drawHome(Game::Home* home, const GUI::Color* color)
{
    
    glColor3f(color->getR(), color->getG(), color->getB());
   
 
    float xloc = home->getPosition()->getX();
    float yloc = home->getPosition()->getY();
    float radius = home->getRadius();
    float worldsize = Game::Robotix::getInstance()->getWorldSize();

    GlDrawCircle( xloc, yloc, radius, 16 );
	GlDrawCircle( xloc+worldsize, yloc, radius, 16 );
	GlDrawCircle( xloc-worldsize, yloc, radius, 16 );
	GlDrawCircle( xloc, yloc+worldsize, radius, 16 );
	GlDrawCircle( xloc, yloc-worldsize, radius, 16 );
		
}


void initGraphics(int argc, char **argv)
{
    glutInit(&argc, argv);
    unsigned int lWindowSize = Game::Robotix::getInstance()->getWindowSize();
    glutInitWindowSize( lWindowSize, lWindowSize );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutCreateWindow( argv[0] );
    glClearColor( 0.1, 0.1, 0.1, 1 );
    glutDisplayFunc( displayFunc );
    glutIdleFunc(idleFunc);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0,1,0,1 );

    float lWorldSize = Game::Robotix::getInstance()->getWorldSize();
    glScalef( 1.0/lWorldSize, 1.0/lWorldSize, 1 ); 
	glPointSize( 4.0 );
    glutMainLoop();
}

