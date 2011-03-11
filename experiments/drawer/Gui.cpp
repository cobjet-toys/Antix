#include "Gui.h"
#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif


void idleFunc()
{
    Network::DrawServer::getInstance()->update();
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

    drawTeams();
    drawPucks();
    drawRobots();

    /*
    //Draw the pucks
    for (Game::PuckIter it = Game::Robotix::getInstance()->getFirstPuck();
            it != Game::Robotix::getInstance()->getLastPuck(); it++)
    {
        (*it).second->draw();
    }
    
    for (std::list<Game::Team*>::iterator it = Game::Robotix::getInstance()->getFirstTeam();
            it != Game::Robotix::getInstance()->getLastTeam();it++)
    {
        (*it).second->getHome()->draw();
        
                for (std::list<Game::Robot*>::iterator rt = (*it).second->getFirstRobot();
	    		rt != (*it).second->getLastRobot(); rt++)
	    {
            (*rt)->draw();
	    }
    }
     * */

    glutSwapBuffers();
    glutTimerFunc( 20, timerFunc, 0);
}

void drawPucks()
{
    glColor3f( 255, 255, 255 ); // white

    glPointSize( 1.0 );

    // pack the puck points into a vertex array for fast rendering
    const size_t len( Network::DrawServer::getInstance()->getPucksCount());

    // keep this buffer around between calls for speed
    static std::vector<GLfloat> pts;
    pts.resize( len * 2 );
    glVertexPointer( 2, GL_FLOAT, 0, &pts[0] );

    int i=0;
    for (Game::PuckIter it = Network::DrawServer::getInstance()->getFirstPuck();
            it != Network::DrawServer::getInstance()->getLastPuck();it++)
    {
        Game::Puck * puck = (*it).second;
        
        pts[2*i+0] = puck->posX;
        pts[2*i+1] = puck->posY;
        i++;
    }

    glDrawArrays( GL_POINTS, 0, len );

    glPointSize( 2.0 );   
}

void drawTeams()
{
    float worldsize = Network::DrawServer::getInstance()->getWorldSize();
    float radius = Network::DrawServer::getInstance()->getHomeRadius();

    for (Game::TeamIter it = Network::DrawServer::getInstance()->getFirstTeam();
            it != Network::DrawServer::getInstance()->getLastTeam();it++)
    {
        Game::Team * team = (*it).second;

        glColor3f( team->colR, team->colG, team->colB );

        GlDrawCircle( team->posX, team->posY, radius, 16 );
        GlDrawCircle( team->posX+worldsize, team->posY, radius, 16 );
        GlDrawCircle( team->posX-worldsize, team->posY, radius, 16 );
        GlDrawCircle( team->posX, team->posY+worldsize, radius, 16 );
        GlDrawCircle( team->posX, team->posY-worldsize, radius, 16 );
    }
}

void drawRobots()
{
    int winsize = Network::DrawServer::getInstance()->getWindowSize();
    float worldsize = Network::DrawServer::getInstance()->getWorldSize();
    float radius = Network::DrawServer::getInstance()->getHomeRadius();
    
    // if robots are smaller than 4 pixels across, draw them as points
    if( (radius * (double)winsize/(double)worldsize) < 2.0 )
    {
        const size_t len( Network::DrawServer::getInstance()->getRobotsCount() );
        // keep this buffer around between calls for speed
        static std::vector<GLfloat> pts;
        static std::vector<GLfloat> colors;
        pts.resize( len * 2 );
        colors.resize( len * 3 );

        glVertexPointer( 2, GL_FLOAT, 0, &pts[0] );

        glEnableClientState( GL_COLOR_ARRAY );
        glColorPointer( 3, GL_FLOAT, 0, &colors[0] );

        int i=0;
        for (Game::RobotIter it = Network::DrawServer::getInstance()->getFirstRobot();
                it != Network::DrawServer::getInstance()->getLastRobot();it++)
        {
            Game::Robot * robot = (*it).second;

            pts[2*i+0] = robot->posX;
            pts[2*i+1] = robot->posY;

            colors[3*i+0] = robot->team->colR;
            colors[3*i+1] = robot->team->colG;
            colors[3*i+2] = robot->team->colB;

            i++;

        }

        glDrawArrays( GL_POINTS, 0, len );
        glDisableClientState( GL_COLOR_ARRAY );
    }
    else 
    {
        for (Game::RobotIter it = Network::DrawServer::getInstance()->getFirstRobot();
                it != Network::DrawServer::getInstance()->getLastRobot();it++)
        {
            //(*it).second->Draw();
        }
    }

}

    /*
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
        glBegin(GL_LINE_LOOP);
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

        //Draw bounding box.
        glBegin(GL_LINE_LOOP);
        glVertex2f(0.0, sin(l_Left)*l_SensRange);
        glVertex2f(l_SensRange, sin(l_Left)*l_SensRange);
        glVertex2f(l_SensRange, sin(l_Right)*l_SensRange);
        glVertex2f(0.0, sin(l_Right)*l_SensRange);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2f(-l_SensRange, -l_SensRange);
        glVertex2f(-l_SensRange, l_SensRange);
        glVertex2f(l_SensRange, l_SensRange);  
        glVertex2f(l_SensRange, -l_SensRange);
        glEnd();
    }

    glPopMatrix();
}
     * */

// utility
void GlDrawCircle( double x, double y, double r, double count )
{
	glBegin(GL_LINE_LOOP);
	for( float a=0; a<(M_PI*2.0); a+=M_PI/count )
		glVertex2f( x + sin(a) * r, y + cos(a) * r );
	glEnd();
}


/*
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
 */


void initGraphics(int argc, char **argv)
{
    glutInit(&argc, argv);
    unsigned int lWindowSize = Network::DrawServer::getInstance()->getWindowSize();
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

    float lWorldSize = Network::DrawServer::getInstance()->getWorldSize();
    glScalef( 1.0/lWorldSize, 1.0/lWorldSize, 1 ); 
    glPointSize( 4.0 );
    glutMainLoop();
}

