#include "Gui.h"
#include "Config.h"

#include <pthread.h>
#include <unistd.h>
#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif


void * listener_function(void* args)
{
	drawServerRef->start();
	return NULL;
}

void idleFunc(){}

void timerFunc(int dummy)
{
    glutPostRedisplay();
}

void displayFunc()
{    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    // ----- Draw Functions ----- //

    drawTeams();
    drawPucks();
    drawRobots(10);
    //drawTest(50, 16, 45.0, 6);
    
    // ----- Draws framerate ----- //
    prevT = currT;
    currT = glutGet(GLUT_ELAPSED_TIME);
    timeIncr += (currT - prevT)/1000.0;
    currentFCount++;
    if(timeIncr > 1.0)
    {
        framerate = currentFCount;
        currentFCount = 0;
        timeIncr--;
    }

    char buf[20];
    sprintf(buf, "Framerate: %d", framerate);
    drawText(buf, 5, 585, false);

    // ------------------------ //
    glutSwapBuffers();
    glutTimerFunc(0, timerFunc, 0);

    // Keeps track of draw counts
    drawCount++;
}

void drawTeams()
{
    unsigned int winsize = drawServerRef->getWindowSize();
    float worldsize      = drawServerRef->getWorldSize();
    float radius         = drawServerRef->getHomeRadius();

    for (Network::TeamIter it = drawServerRef->getFirstTeam(); it != drawServerRef->getLastTeam(); it++)
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

    unsigned int winsize = drawServerRef->getWindowSize();

    // pack the puck points into a vertex array for fast rendering
    const size_t len(drawServerRef->getPucksCount());

    // keep this buffer around between calls for speed
    static std::vector<GLfloat> pts;
    pts.resize(len * 2);
    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &pts[0]);

    int i = 0;
    for (Network::PuckIter it = drawServerRef->getFirstPuck(); it != drawServerRef->getLastPuck(); it++)
    {    
        Math::Position * puckPos = (*it)->getPosition();
        
        pts[i]      = puckPos->getX();
        pts[i + 1]  = puckPos->getY();
        i += 2;
    }

    // Draw the array of vertices
    glDrawArrays(GL_POINTS, 0, len);

    // Deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);

    char tmpBuf[16];
    sprintf(tmpBuf, "Pucks  #: %u", len);
    drawText(tmpBuf, 5, 570, false);
}

void drawRobots(int edgePoints)
{
    unsigned int winsize   = drawServerRef->getWindowSize();
    float worldsize        = drawServerRef->getWorldSize();
    float radius           = drawServerRef->getHomeRadius();

    glPointSize(2.0);
    glColor3f(255, 255, 255);
    
    // if robots are smaller than 4 pixels across, draw them as points
    //if( (radius * (double)winsize/(double)worldsize) < 2.0 )
    if(true)
    {
        const size_t len(drawServerRef->getRobotsCount()); 
    	
        // keep this buffer around between calls for speed
        static std::vector<GLfloat> pts, sPts, lPts;
        //static std::vector<GLfloat> colors;
        pts.resize(len * 2);
        sPts.resize(len * edgePoints * 2);
        lPts.resize(len * 4);
        //colors.resize(len * 3);

        //glEnableClientState(GL_COLOR_ARRAY);
        //glColorPointer(3, GL_FLOAT, 0, &colors[0]);

        int i = 0;
        for (Network::RobotIter it = drawServerRef->getFirstRobot(); it != drawServerRef->getLastRobot(); it++)
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
                sPts[(edgePoints*2*i) + (2*a)]     = robotPos->getX() + xRobotVals[(int)(a*360/edgePoints)];
                sPts[(edgePoints*2*i) + (2*a) + 1] = robotPos->getY() + yRobotVals[(int)(a*360/edgePoints)];
            }
    
            // --- NEEDS A HEADING VALUE FOR PASSED IN ROBOTS -- //
            // Sets the heading line points for each robot
            //lPts[(4*i) + 0] = values[i];
            //lPts[(4*i) + 1] = values[i];
            //lPts[(4*i) + 2] = values[i] + xRobotVals[(int)(*it).second->getHeading()];
            //lPts[(4*i) + 3] = values[i] + yRobotVals[(int)(*it).second->getHeading()];

            //colors[3*i + 0] = 255;
            //colors[3*i + 1] = 255;
            //colors[3*i + 2] = 255;

            //char tmpBuf[40];
            //sprintf(tmpBuf, "Robots[%d]: [%f, %f]", i, robotPos->getX(), robotPos->getY());
            //drawText(tmpBuf, 5, 540 - (15*i), false);

            i++;
        }
        glEnd();

        // Activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);

        //glVertexPointer(2, GL_FLOAT, 0, &pts[0]);        
        //glDrawArrays(GL_POINTS, 0, len);

        // Draw robot circles
        glVertexPointer(2, GL_FLOAT, 0, &sPts[0]);        
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
        //glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        char tmpBuf[16];
        sprintf(tmpBuf, "Robots #: %u", len);
        drawText(tmpBuf, 5, 555, false);
    }
    else 
    {
        for (Network::RobotIter it = drawServerRef->getFirstRobot(); it != drawServerRef->getLastRobot();it++)
        {
        	if (!(*it))
        	{
        		continue;
        	}
        	
            //(*it).second->draw();
        }
    }

}

void drawText(char* text, float x, float y, bool absolute)
{
    glColor3f(0, 255, 255);
    
    unsigned int winsize = drawServerRef->getWindowSize();
    void* font = GLUT_BITMAP_9_BY_15;

    if(absolute){ glRasterPos2f(x, y); }
    else{ glRasterPos2f(left + x*(abs(left - right)/(float)winsize), bottom + y*(abs(bottom - top)/(float)winsize)); }
    while(*text) 
    {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

// Draws robots and their orientation
void drawTest(int robotCount, int edgePoints, float viewAngle, int viewEdgePoints)
{
    // Start clock timer (testing purposes)
    //clock_t start = clock();

    int windowSize = drawServerRef->getWindowSize();

    if(drawCount%20 == 19){angleOffset++;}

    if(abs(left - right) < 8500)
    {
        // Set colour to WHITE
        glColor3f(255, 255, 255);

        // Initialize value arrays
        GLfloat values[robotCount];
        for(size_t i = 0; i < robotCount; i++)
        {    
           values[i] = windowSize/double(robotCount)*i;
        }
     
        // Set Robot body and orientation points
        GLfloat cVertices[robotCount*edgePoints*2];
        GLfloat lVertices[robotCount*4];
        for(size_t i = 0; i < robotCount; i++)
        {    
            // Set robots circle vertex values
            for(size_t a = 0; a < edgePoints; a++)
            {
                cVertices[(edgePoints*2*i) + (2*a)]     = values[i] + xRobotVals[(int)(a*360/edgePoints)];
                cVertices[(edgePoints*2*i) + (2*a) + 1] = values[i] + yRobotVals[(int)(a*360/edgePoints)];
                //printf("vertices[%d]: (%f, %f) \n", (20*i) + (2*a), vertices[(20*i) + (2*a)], vertices[(20*i) + (2*a) + 1]);
            }

            // Set robots orientation line
            lVertices[(4*i) + 0] = values[i];
            lVertices[(4*i) + 1] = values[i];
            lVertices[(4*i) + 2] = values[i] + xRobotVals[(int)((i + (angleOffset%robotCount))*(360.0/robotCount))%360];
            lVertices[(4*i) + 3] = values[i] + yRobotVals[(int)((i + (angleOffset%robotCount))*(360.0/robotCount))%360];   
        }
    
        // Activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, cVertices);

        // Draw a line strip (robot circle)
        for(int x = 0; x < robotCount; x++)
        {
            glDrawArrays(GL_LINE_LOOP, edgePoints*x, edgePoints);
        }

        glVertexPointer(2, GL_FLOAT, 0, lVertices);

        // Draw a line strip (robot circle)
        for(int x = 0; x < robotCount; x++)
        {
            glDrawArrays(GL_LINES, 2*x, 2);
        }

        if(drawFOV)
        {
            // Sets colour to RED
            glColor3f(255, 0, 0);
            
            // Set FOV points
            GLfloat sVertices[robotCount*(viewEdgePoints + 1)*2];
            float current = 0.0;
            float incr = viewAngle/viewEdgePoints;
            for(size_t i = 0; i < robotCount; i++)
            { 
                sVertices[((viewEdgePoints + 1)*2*i) + 0] = values[i];
                sVertices[((viewEdgePoints + 1)*2*i) + 1] = values[i];

                current = (int)((i + (angleOffset%robotCount))*(360.0/robotCount)) - viewAngle/2;
                if(current < 0){ current += 360; }

                for(size_t a = 1; a <= viewEdgePoints; a++)
                {
                    sVertices[((viewEdgePoints + 1)*2*i) + (2*a) + 0] = values[i] + xSensVals[(int)current%360];
                    sVertices[((viewEdgePoints + 1)*2*i) + (2*a) + 1] = values[i] + ySensVals[(int)current%360];
                    //printf("vertices[%d]: (%f, %f) \n", (20*i) + (2*a), vertices[(20*i) + (2*a)], vertices[(20*i) + (2*a) + 1]);
        
                    current += incr;
                }    
            }

            //printf("drawCount-360: %d \n", (int)(drawCount%360));

            glVertexPointer(2, GL_FLOAT, 0, sVertices);
            
            // Draw a line strip (robot sensor field)
            for(int x = 0; x < robotCount; x++)
            {
                glDrawArrays(GL_LINE_LOOP, (viewEdgePoints + 1)*x, (viewEdgePoints + 1));
            }
        }

        // deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else
    {
        // Set colour to RED
        glColor3f(255, 255, 255);

        glPointSize(1.0);

        // Initialize value arrays
        GLfloat values[robotCount];
        for(size_t i = 0; i < robotCount; i++)
        {    
           values[i] = windowSize/double(robotCount)*i;
        }
     
        // Specify vertex coords
        GLfloat pts[robotCount*2];
        for(size_t i = 0; i < robotCount; i++)
        {    
            pts[2*i]     = values[i];
            pts[2*i + 1] = values[i];
        }

        // Activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, pts);

        // Draw a line strip (robot circle)
        glDrawArrays(GL_POINTS, 0, robotCount);

        // deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    char tmpBuf[30];
    sprintf(tmpBuf, "Width/Height: [%d, %d]", abs(left - right), abs(bottom - top));
    drawText(tmpBuf, 600, 0, true);

    // Print time in (ms) that it took to fetch data 
    //double elapsed = (clock() - start);
    //printf("Draw func #1: %fms\n", elapsed);
}

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

void initializePositionLookupArrays(double radius, int count, GLfloat *xVals, GLfloat *yVals)
{
    int index = 0;
    for(float a = 0; a < (M_PI*2.0); a += (M_PI*2.0)/count)
    {
        if(index < count)
        {
            xVals[index] = sin(a) * radius;
            yVals[index] = cos(a) * radius;
            //printf("Pos vals for (deg %f): [%f, %f] \n", (a/(M_PI*2.0))*360.0, xVals[index], yVals[index]);
            index++;
        }
    }
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
                drawServerRef->updateViewRange(left, top, right, bottom);
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
                drawServerRef->updateViewRange(left, top, right, bottom);
                break;
            default:
                break;
        }
    }
}

void mouseMotionHandler(int x, int y)
{
    float windowSize = drawServerRef->getWindowSize();

    if(inDrag)
    {
        float xD = (float)(xCur - x)*(abs(left - right)/windowSize);
        float yD = (float)(y - yCur)*(abs(bottom - top)/windowSize);

        xCur = x;
        yCur = y;
        
        switch(actionType)
        {
            case 0: // Translate
                left += xD;
                right += xD;            
                bottom += yD;
                top += yD;
                break;
            case 1: // Zoom
                if(yD < 0) // Zoom out
                {                    
                    left += yD;
                    right -= yD;
                    bottom += yD;
                    top -= yD;
                }
                else if(yD > 0) // Zoom in
                {
                    if((right - yD) - (left + yD) > 10) // Zoom limit
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
    }
}

void keyEventHandler(unsigned char key, int x, int y)
{
    switch(tolower(key))
    {
        case 'f':
            drawFOV = !drawFOV;
            break;
    }

    printf("Key Pressed: %c \n", key);
}

void initGraphics(int argc, char **argv)
{    
    unsigned int lWindowSize = drawServerRef->getWindowSize();
    float lWorldSize = drawServerRef->getWorldSize();

    glutInit(&argc, argv);
    glutInitWindowSize(lWindowSize, lWindowSize);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow(argv[0]);
    glClearColor(0.1, 0.1, 0.1, 1);
    glutDisplayFunc(displayFunc);
    glutIdleFunc(idleFunc);
    glutMouseFunc(mouseClickHandler);
    glutMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyEventHandler);
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
    initializePositionLookupArrays(5, 360, xRobotVals, yRobotVals);
    initializePositionLookupArrays(20, 360, xSensVals, ySensVals);

    pthread_t thread1;  
	int ret = pthread_create(&thread1, NULL, listener_function, NULL);
	if(ret != 0)
	{
	    perror("pthread failed: can't start client listener.\n");
	} 
    
    glutMainLoop();
}

