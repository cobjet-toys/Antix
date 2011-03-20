#include "Gui.h"
#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif


void printRobots()
{
	printf("printRobots()\n");
	for (Network::DrawServer::RobotIter it = Network::DrawServer::getInstance()->getFirstRobot();
             it != Network::DrawServer::getInstance()->getLastRobot();it++)
        {
        	printf("%d: %d\n", (*it).first, (*it).second != 0);
        }
}

void idleFunc()
{
    //Network::DrawServer::getInstance()->update();   
}

void timerFunc(int dummy)
{
    glutPostRedisplay();
}

void displayFunc()
{    
    Network::DrawServer::getInstance()->update();  

    //printf("displayFunc()::");
    //printRobots();
    
    glClear( GL_COLOR_BUFFER_BIT );
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    drawTeams();
    drawPucks();
    drawRobots();
    //drawTest();

    glutSwapBuffers();
    glutTimerFunc(0, timerFunc, 0);
}

void drawTeams()
{
    unsigned int winsize = Network::DrawServer::getInstance()->getWindowSize();
    float worldsize = Network::DrawServer::getInstance()->getWorldSize();
    float radius = Network::DrawServer::getInstance()->getHomeRadius();

    for (Network::DrawServer::TeamIter it = Network::DrawServer::getInstance()->getFirstTeam();
         it != Network::DrawServer::getInstance()->getLastTeam(); it++)
    {
        Math::Position * homePos = (*it).second->getHome()->getPosition();

        glColor3f(255, 255, 255);

        GlDrawCircle(homePos->getX()/winsize, homePos->getY()/winsize, radius/winsize, 16);
    }
}

void drawPucks()
{
    glColor3f(255, 0, 0); // RED

    glPointSize(4.0);

    unsigned int winsize = Network::DrawServer::getInstance()->getWindowSize();

    // pack the puck points into a vertex array for fast rendering
    const size_t len( Network::DrawServer::getInstance()->getPucksCount());

    // keep this buffer around between calls for speed
    static std::vector<GLfloat> pts;
    pts.resize(len * 2);
    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &pts[0]);

    int i=0;
    for (Network::DrawServer::PuckIter it = Network::DrawServer::getInstance()->getFirstPuck();
         it != Network::DrawServer::getInstance()->getLastPuck(); it++)
    {    
        Math::Position * puckPos = (*it).second->getPosition();
        
        pts[i]   = puckPos->getX()/winsize;
        pts[i+1] = puckPos->getY()/winsize;
        i += 2;
    }

    // Draw the array of vertices
    glDrawArrays(GL_POINTS, 0, len);

    // Deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawRobots()
{
    unsigned int winsize   = Network::DrawServer::getInstance()->getWindowSize();
    float worldsize        = Network::DrawServer::getInstance()->getWorldSize();
    float radius           = Network::DrawServer::getInstance()->getHomeRadius();

    glPointSize(2.0);
    glColor3f(255, 255, 255);
    
    // if robots are smaller than 4 pixels across, draw them as points
    //if( (radius * (double)winsize/(double)worldsize) < 2.0 )
    if(true)
    {
        const size_t len( Network::DrawServer::getInstance()->getRobotsCount()); 
    	
        // keep this buffer around between calls for speed
        static std::vector<GLfloat> pts;
        static std::vector<GLfloat> colors;
        pts.resize(len * 2);
        colors.resize(len * 3);

        // Activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &pts[0]);

        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, &colors[0]);

        int i = 0;
        for (Network::DrawServer::RobotIter it = Network::DrawServer::getInstance()->getFirstRobot();
             it != Network::DrawServer::getInstance()->getLastRobot();it++)
        {
           	if(!(*it).second)
          	{
            		printf("No robot @ %d", (*it).first);
            		continue;
          	}
        	
            Math::Position * robotPos = (*it).second->getPosition();
            assert(robotPos);

            pts[2*i + 0] = robotPos->getX()/winsize;
            pts[2*i + 1] = robotPos->getY()/winsize;

            colors[3*i + 0] = 255;
            colors[3*i + 1] = 255;
            colors[3*i + 2] = 255;

            i++;
        }
        glEnd();

        glDrawArrays(GL_POINTS, 0, len);

        // Deactivate vertex arrays after drawing
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else 
    {
        for (Network::DrawServer::RobotIter it = Network::DrawServer::getInstance()->getFirstRobot();
                it != Network::DrawServer::getInstance()->getLastRobot();it++)
        {
        	if (!(*it).second)
        	{
        		printf("No robot @ %d", (*it).first);
        		continue;
        	}
        	
            (*it).second->draw();
        }
    }

}

void drawText(char* text, float x, float y)
{
    unsigned int winsize = Network::DrawServer::getInstance()->getWindowSize();
    void* font = GLUT_BITMAP_9_BY_15;
    glRasterPos2f(x/winsize, y/winsize);
    while(*text) 
    {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

// Draws 1000000 vertices using 2 methods, and compares the drawtimes
void drawTest()
{
    unsigned int numValues = 1000000;
  
    // Initialize value arrays
    GLfloat value1[numValues/2];
    GLfloat value2[numValues/2];
    for(size_t i = 0; i < numValues/2; i++)
    {    
       value1[i] = 0.0 + 1/double(numValues)*i;
       value2[i] = 0.5 + 1/double(numValues)*i;
    }
  
    // Set point size to 5.0
    glPointSize(5.0);

    // Set colour to BLUE
    glColor3f(0, 0, 255);

    // Start clock timer (testing purposes)
    clock_t start = clock();

    // Draw vertex points incrementally
    glBegin(GL_POINTS);
	  for (size_t i = 0; i < numValues/2; i++)
    {    
       glVertex2f(value1[i], value1[i]);
    }
    glEnd();

    // Print time in (ms) that it took to fetch data 
    double elapsed = (clock() - start);
    printf("Draw func #1: %fms\n", elapsed);

    // Set colour to RED
    glColor3f(255, 0, 0);

    // Start clock timer (testing purposes)
    start = clock();

    // Specify vertex coords
    GLfloat vertices[numValues/2];
    for (size_t i = 0; i < numValues/2; i++)
    {    
       vertices[2*i]     = value2[i];
       vertices[2*i + 1] = value2[i];
    }

    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    // draw a cube
    glDrawArrays(GL_POINTS, 0, numValues/2);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);

    // Print time in (ms) that it took to fetch data 
    double elapsed2 = (clock() - start);
    printf("Draw func #2: %fms\n", elapsed2);
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
void GlDrawCircle(double x, double y, double r, double count)
{
	  glBegin(GL_LINE_LOOP);
	  for(float a=0; a < (M_PI*2.0); a += M_PI/count)
    {
		    glVertex2f(x + sin(a) * r, y + cos(a) * r);
    }
	  glEnd();
}


void initGraphics(int argc, char **argv)
{
    glutInit(&argc, argv);
    unsigned int lWindowSize = Network::DrawServer::getInstance()->getWindowSize();
    glutInitWindowSize( lWindowSize, lWindowSize );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutCreateWindow( argv[0] );
    glClearColor( 0.1, 0.1, 0.1, 1 );
    glutDisplayFunc(displayFunc);
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

