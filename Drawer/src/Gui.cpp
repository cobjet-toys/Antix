#include "Gui.h"
#include "Config.h"

#include <pthread.h>
#include <unistd.h>
#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif

static int threaded = 1;

void * listener_function(void* args)
{
	Network::DrawServer::getInstance()->start();
	return NULL;
}

void idleFunc(){}

void timerFunc(int dummy)
{
    glutPostRedisplay();
}

void displayFunc()
{    
    //printf("displayFunc()::");
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    drawTeams();
    drawPucks();
    drawRobots(20);
    //drawTest(100, 20);

    char buf[20];
    sprintf(buf, "Frame Count: %d", drawCount++);
    drawText(buf, 5, 585);

    glutSwapBuffers();
    glutTimerFunc(0, timerFunc, 0);
}

void drawTeams()
{
    unsigned int winsize = Network::DrawServer::getInstance()->getWindowSize();
    float worldsize      = Network::DrawServer::getInstance()->getWorldSize();
    float radius         = Network::DrawServer::getInstance()->getHomeRadius();

    for (Network::TeamIter it = Network::DrawServer::getInstance()->getFirstTeam();
         it != Network::DrawServer::getInstance()->getLastTeam(); it++)
    {
        glColor3f(255, 255, 255);

        Math::Position * homePos = (*it)->getPosition();
        GlDrawCircle(homePos->getX(), homePos->getY(), radius, 16);
    }
}

void drawPucks()
{
    glColor3f(255, 0, 0); // RED

    glPointSize(4.0);

    unsigned int winsize = Network::DrawServer::getInstance()->getWindowSize();

    // pack the puck points into a vertex array for fast rendering
    const size_t len(Network::DrawServer::getInstance()->getPucksCount());

    // keep this buffer around between calls for speed
    static std::vector<GLfloat> pts;
    pts.resize(len * 2);
    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &pts[0]);

    int i=0;
    for (Network::PuckIter it = Network::DrawServer::getInstance()->getFirstPuck();
         it != Network::DrawServer::getInstance()->getLastPuck(); it++)
    {    
        Math::Position * puckPos = (*it)->getPosition();
        
        pts[i]   = puckPos->getX();
        pts[i+1] = puckPos->getY();
        i += 2;
    }

    // Draw the array of vertices
    glDrawArrays(GL_POINTS, 0, len);

    // Deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);

    char tmpBuf[16];
    sprintf(tmpBuf, "Pucks  #: %u", len);
    drawText(tmpBuf, 5, 570);
}

void drawRobots(int edgePoints)
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
        static std::vector<GLfloat> pts, sPts, lPts;
        static std::vector<GLfloat> colors;
        pts.resize(len * 2);
        sPts.resize(len * edgePoints * 2);
        lPts.resize(len * 4);
        colors.resize(len * 3);

        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, &colors[0]);

        int i = 0;
        for (Network::RobotIter it = Network::DrawServer::getInstance()->getFirstRobot();
             it != Network::DrawServer::getInstance()->getLastRobot();it++)
        {
           	if(!(*it))
          	{
        		continue;
          	}
        	
            Math::Position * robotPos = (*it)->getPosition();
            if (!robotPos) continue;

            //pts[2*i + 0] = robotPos->getX();
            //pts[2*i + 1] = robotPos->getY();

            // Sets the circle points for each robot
            for(size_t a = 0; a < edgePoints; a++)
            {
                sPts[(edgePoints*2*i) + (2*a)]     = robotPos->getX() + xVals[(int)(a*360/edgePoints)];
                sPts[(edgePoints*2*i) + (2*a) + 1] = robotPos->getX() + yVals[(int)(a*360/edgePoints)];
            }
    
            // --- NEEDS A HEADING VALUE FOR PASSED IN ROBOTS -- //
            // Sets the heading line points for each robot
            //lPts[(4*i) + 0] = values[i];
            //lPts[(4*i) + 1] = values[i];
            //lPts[(4*i) + 2] = values[i] + xVals[(int)(*it).second->getHeading()];
            //lPts[(4*i) + 3] = values[i] + yVals[(int)(*it).second->getHeading()];

            colors[3*i + 0] = 255;
            colors[3*i + 1] = 255;
            colors[3*i + 2] = 255;

            i++;
        }
        glEnd();

        // Activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);

        // Draw robot circles
        glVertexPointer(2, GL_FLOAT, 0, &sPts[0]);        
        //glDrawArrays(GL_POINTS, 0, len);
        for (int x = 0; x < i; x++)
        {
            glDrawArrays(GL_LINE_LOOP, x*edgePoints, edgePoints);
        }

        // Draw robot orientation lines
        //glVertexPointer(2, GL_FLOAT, 0, &lPts[0]);
        //for (int x = 0; x < i; x++)
        //{
        //    glDrawArrays(GL_LINE_LOOP, x*2, 2);
        //}

        // Deactivate vertex arrays after drawing
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        char tmpBuf[16];
        sprintf(tmpBuf, "Robots #: %u", len);
        drawText(tmpBuf, 5, 555);
    }
    else 
    {
        for (Network::RobotIter it = Network::DrawServer::getInstance()->getFirstRobot();
                it != Network::DrawServer::getInstance()->getLastRobot();it++)
        {
        	if (!(*it))
        	{
        		continue;
        	}
        	
            //(*it).second->draw();
        }
    }

}

void drawText(char* text, float x, float y)
{
    unsigned int winsize = Network::DrawServer::getInstance()->getWindowSize();
    void* font = GLUT_BITMAP_9_BY_15;
    glRasterPos2f(x, y);
    while(*text) 
    {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

// Draws 1000000 vertices using 2 methods, and compares the drawtimes
void drawTest(int robotCount, int edgePoints)
{
    /*unsigned int numValues = 1000000;
  
    // Initialize value arrays
    GLfloat value1[numValues/2];
    GLfloat value2[numValues/2];
    for(size_t i = 0; i < numValues/2; i++)
    {    
       value1[i] = 0.0 + 600/double(numValues)*i;
       value2[i] = 300 + 600/double(numValues)*i;
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
    //printf("Draw func #1: %fms\n", elapsed);

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
    //printf("Draw func #2: %fms\n", elapsed2);*/

    // Set colour to RED
    glColor3f(255, 0, 0);

    int numValues = robotCount; // Number of robots

    // Initialize value arrays
    GLfloat values[numValues];
    for(size_t i = 0; i < numValues; i++)
    {    
       values[i] = 600/double(numValues)*i;
    }
 
    // Specify vertex coords
    GLfloat sVertices[numValues*edgePoints*2];
    GLfloat lVertices[numValues*4];
    for(size_t i = 0; i < numValues; i++)
    {    
        for(size_t a = 0; a < edgePoints; a++)
        {
            sVertices[(edgePoints*2*i) + (2*a)]     = values[i] + xVals[(int)(a*360/edgePoints)];
            sVertices[(edgePoints*2*i) + (2*a) + 1] = values[i] + yVals[(int)(a*360/edgePoints)];
            //printf("vertices[%d]: (%f, %f) \n", (20*i) + (2*a), vertices[(20*i) + (2*a)], vertices[(20*i) + (2*a) + 1]);
        }

        lVertices[(4*i) + 0] = values[i];
        lVertices[(4*i) + 1] = values[i];
        lVertices[(4*i) + 2] = values[i] + xVals[(int)(i*(360.0/numValues))];
        lVertices[(4*i) + 3] = values[i] + yVals[(int)(i*(360.0/numValues))];        
    }

    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, sVertices);

    // Draw a line strip (robot circle)
    for(int x = 0; x < numValues; x++)
    {
        glDrawArrays(GL_LINE_LOOP, edgePoints*x, edgePoints);
    }

    glVertexPointer(2, GL_FLOAT, 0, lVertices);

    // Draw a line strip (robot circle)
    for(int x = 0; x < numValues; x++)
    {
        glDrawArrays(GL_LINES, 2*x, 2);
    }

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);  
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
    for(float a = 0; a < (M_PI*2.0); a += M_PI/count)
    {
        glVertex2f(x + sin(a) * r, y + cos(a) * r);
    }
    glEnd();
}

void initializePositionLookupArrays(double radius)
{
    int index = 0;
    for(float a = 0; a < (M_PI*2.0); a += (M_PI*2.0)/360.0)
    {
        if(index < 360)
        {
            xVals[index] = sin(a) * radius;
            yVals[index] = cos(a) * radius;
            //printf("Pos vals for (deg %f): [%f, %f] \n", (a/(M_PI*2.0))*360.0, xVals[index], yVals[index]);
            index++;
        }
    }
    printf("Index: %d \n", index); 
}

void mouseClickHandler(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        actionType = 0; // Drag -- Translation action
        switch(state)
        {
            case GLUT_DOWN:
                //printf("Mouse Down @ [%d, %d]\n", x, y);
                inDrag = true;
                xInit = x;
                yInit = y;
                xCur = x;
                yCur = y;
                break;
            case GLUT_UP:
                //printf("Mouse Up   @ [%d, %d]\n", x, y);
                inDrag = false;     
                xEnd = x;
                yEnd = y;
                break;
            default:
                break;
        }
    }
    else if(button == GLUT_RIGHT_BUTTON)
    {
        actionType = 1; // Zoom -- Scaling action
        switch(state)
        {
            case GLUT_DOWN:
                //printf("Mouse Down @ [%d, %d]\n", x, y);
                inDrag = true;
                xInit = x;
                yInit = y;
                xCur = x;
                yCur = y;
                break;
            case GLUT_UP:
                //printf("Mouse Up   @ [%d, %d]\n", x, y);
                inDrag = false;     
                xEnd = x;
                yEnd = y;
                break;
            default:
                break;
        }
    }   
}

void mouseMotionHandler(int x, int y)
{
    if(inDrag)
    {
        
        int xD = xCur - x;
        int yD = y - yCur;

        xCur = x;
        yCur = y;

        float ratio = 1.0;

        switch(actionType)
        {
            case 0:
                ratio = abs(left - right)/600.0;
                left += xD;//*ratio;
                right += xD;//*ratio;            
                bottom += yD;//*ratio;
                top += yD;//*ratio;
                break;
            case 1:
                if(yD < 0)
                {                    
                    left += yD;
                    right -= yD;
                    bottom += yD;
                    top -= yD;
                }
                else if(yD > 0)
                {
                    if((right - yD) - (left + yD) > 10)
                    {
                        left += yD;
                        right -= yD;
                        bottom += yD;
                        top -= yD;
                    }
                }                
                //printf("Zoom Level: %f \n", zoomLevel);
                break;
        }
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(left, right, bottom, top);
        //glScalef(zoomLevel, zoomLevel, 1.0);
    }
}

void initGraphics(int argc, char **argv)
{    
    unsigned int lWindowSize = Network::DrawServer::getInstance()->getWindowSize();
    float lWorldSize = Network::DrawServer::getInstance()->getWorldSize();

    glutInit(&argc, argv);
    glutInitWindowSize(lWindowSize, lWindowSize);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow(argv[0]);
    glClearColor(0.1, 0.1, 0.1, 1);
    glutDisplayFunc(displayFunc);
    glutIdleFunc(idleFunc);
    glutMouseFunc(mouseClickHandler);
    glutMotionFunc(mouseMotionHandler);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    // Set viewport values
    left = 0;
    right = lWindowSize;
    bottom = 0;
    top = lWindowSize;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);    
    glScalef(1.0/lWorldSize, 1.0/lWorldSize, 1);

    // Initialize look up arrays
    initializePositionLookupArrays(5.0);

    pthread_t thread1;  
	int ret = pthread_create(&thread1, NULL, listener_function, NULL);
	if(ret != 0)
	{
	    perror("pthread failed: can't start client listener.\n");
	} 
    
    glutMainLoop();
}

