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
    if(drawTeam)
    {
        drawTeams();
    }
    if(drawPuck)
    {
        drawPucks();
    }
    if(drawRobot)
    {
        drawRobots(robotEdgeCount);
    }
    if(drawBoundary)
    {
        drawBoundaries();
    }
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
    drawText(buf, 5, drawServerRef->getWindowSize() - 15, false);

    char buf2[30];
    sprintf(buf2, "Width/Height: [%d, %d], robotEdgeCount: %d", (int)(right - left), (int)(top - bottom), robotEdgeCount);
    drawText(buf2, 5, drawServerRef->getWindowSize() - 30, false);

    char buf3[30];
    sprintf(buf3, "Mouse XY: [%d, %d]", (int)xTrue, (int)yTrue);
    drawText(buf3, 5, drawServerRef->getWindowSize() - 45, false);

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

    int i = 0;
    for (Network::TeamIter it = drawServerRef->getFirstTeam(); it != drawServerRef->getLastTeam(); it++)
    {
        glColor3f(colorArray[(i*3)], colorArray[(i*3) + 1], colorArray[(i*3) + 2]);

        Math::Position * homePos = (*it)->getPosition();
        GlDrawCircle(homePos->getX(), homePos->getY(), radius, 16);

        i++;
    }
}

void drawPucks()
{
    glColor3f(1.0, 1.0, 1.0); // White

    glPointSize(puckSize);

    unsigned int winsize = drawServerRef->getWindowSize();

    // pack the puck points into a vertex array for fast rendering
    const size_t len(drawServerRef->getPucksCount());

    // keep this buffer around between calls for speed
    static std::vector<GLfloat> pts;
    pts.resize(len * 2);

    // Activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &pts[0]);

    int i = 0, d = 0;
    for (Network::PuckIter it = drawServerRef->getFirstPuck(); it != drawServerRef->getLastPuck(); it++)
    {    
        Math::Position * puckPos = (*it)->getPosition();
        
        int xPos = (int)puckPos->getX();
        int yPos = (int)puckPos->getY();

        if(xPos > left - 10 && xPos < right + 10 && xPos != -1)
        {
           if(yPos > bottom - 10 && yPos < top + 10)
           {
                pts[(d*2) + 0]  = puckPos->getX();
                pts[(d*2) + 1]  = puckPos->getY();
                d++;
            }
        }        
        i++;
    }

    // Draw the array of vertices
    glDrawArrays(GL_POINTS, 0, d);

    // Deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);

    char tmpBuf[40];
    sprintf(tmpBuf, "Pucks  #: %u, Drawn #: %d", i, d);
    drawText(tmpBuf, 5, drawServerRef->getWindowSize() - 60, false);
}

void drawRobots(int edgePoints)
{
    unsigned int winsize   = drawServerRef->getWindowSize();
    float worldsize        = drawServerRef->getWorldSize();
    float radius           = drawServerRef->getHomeRadius();

    glPointSize(1.0);

    const size_t len(drawServerRef->getRobotsCount()); 

    int robotsPerTeam = len/drawServerRef->getTeamsCount();

    // keep this buffer around between calls for speed
    static std::vector<GLfloat> pts, sPts, lPts;
    static std::vector<GLfloat> colors;
    pts.resize(len * 2);
    sPts.resize(len * edgePoints * 2);
    lPts.resize(len * 4);
    colors.resize(len * 3);

    int i = 0, d = 0;
    if(abs(left - right) < cutOffRange)
    {
        for (Network::RobotIter it = drawServerRef->getFirstRobot(); it != drawServerRef->getLastRobot(); it++)
        {
           	if(!(*it)){continue;}
        	
            Math::Position * robotPos = (*it)->getPosition();
            if (!robotPos) continue;

            int xPos = (int)robotPos->getX();
            int yPos = (int)robotPos->getY();

            if(xPos > left - 10 && xPos < right + 10 && xPos != -1)
            {
                if(yPos > bottom - 10 && yPos < top + 10)
                {
                    // Sets the circle points for each robot                
                    for(size_t a = 0; a < edgePoints; a++)
                    {
                        sPts[(edgePoints*2*d) + (2*a)]     = robotPos->getX() + xRobotVals[(int)(a*360/edgePoints)];
                        sPts[(edgePoints*2*d) + (2*a) + 1] = robotPos->getY() + yRobotVals[(int)(a*360/edgePoints)];
                    }

                    int angle = (int)((robotPos->getOrient()/(2*M_PI))*360);
    
                    // Sets the heading line points for each robot
                    lPts[(4*d) + 0] = robotPos->getX();
                    lPts[(4*d) + 1] = robotPos->getY();
                    lPts[(4*d) + 2] = robotPos->getX() + xRobotVals[angle];
                    lPts[(4*d) + 3] = robotPos->getY() + yRobotVals[angle];

                    int colorIndex = ((*it)->getId() + 1)/robotsPerTeam;

                    colors[3*d + 0] = colorArray[(colorIndex*3) + 0];
                    colors[3*d + 1] = colorArray[(colorIndex*3) + 1];
                    colors[3*d + 2] = colorArray[(colorIndex*3) + 2];

                    d++;
                }
            }
    
            i++;
        }

        // Activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
       
        // Draw robot circles
        glVertexPointer(2, GL_FLOAT, 0, &sPts[0]);        
        for (int x = 0; x < d; x++)
        {
            glColor3f(colors[(x*3)], colors[(x*3) + 1], colors[(x*3) + 2]);
            glDrawArrays(GL_LINE_LOOP, x*edgePoints, edgePoints);     
        }

        // Draw robot orientation lines
        glVertexPointer(2, GL_FLOAT, 0, &lPts[0]);
        for (int x = 0; x < d; x++)
        {
            glColor3f(colors[(x*3)], colors[(x*3) + 1], colors[(x*3) + 2]);
            glDrawArrays(GL_LINE_LOOP, x*2, 2);
        }

        // Deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else 
    {
        for (Network::RobotIter it = drawServerRef->getFirstRobot(); it != drawServerRef->getLastRobot(); it++)
        {
           	if(!(*it)){continue;}
        	
            Math::Position * robotPos = (*it)->getPosition();
            if (!robotPos) continue;

            pts[(2*i) + 0] = robotPos->getX();
            pts[(2*i) + 1] = robotPos->getY();

            int colorIndex = ((*it)->getId() + 1)/robotsPerTeam;

            colors[3*i + 0] = colorArray[(colorIndex*3) + 0];
            colors[3*i + 1] = colorArray[(colorIndex*3) + 1];
            colors[3*i + 2] = colorArray[(colorIndex*3) + 2];

            i++;
            d++;
        }

        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, &colors[0]);

        // Activate and specify pointer to vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &pts[0]);

        // Draw a line strip (robot circle)
        glDrawArrays(GL_POINTS, 0, i);

        // deactivate vertex arrays after drawing
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }

    char tmpBuf[40];
    sprintf(tmpBuf, "Robots #: %d, Drawn #: %d", i, d);
    drawText(tmpBuf, 5, drawServerRef->getWindowSize() - 75, false);
}

void drawBoundaries()
{
    float worldSize = drawServerRef->getWorldSize();
    size_t serverCount = drawServerRef->getGridCount();

    int gridWidth = worldSize/serverCount;

    glColor3f(1.0, 1.0, 1.0);

    for(int x = 0; x < serverCount; x++)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2f(x*gridWidth, 0);
        glVertex2f(x*gridWidth, worldSize);
        glVertex2f(x*gridWidth + gridWidth, worldSize);
        glVertex2f(x*gridWidth + gridWidth, 0);
        glEnd();
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
    int windowSize = drawServerRef->getWindowSize();

    if(drawCount%20 == 19){angleOffset++;}

    if(right - left < cutOffRange)
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
                    
                    current += incr;
                }    
            }

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

void initializeColorArray(GLfloat *colors)
{
    // Sets some start colours
    // Red
    colors[0] = 1.0;
    colors[1] = 0.0;
    colors[2] = 0.0;

    // Green
    colors[3] = 0.0;
    colors[4] = 1.0;
    colors[5] = 0.0;

    // Blue
    colors[6] = 0.0;
    colors[7] = 0.0;
    colors[8] = 1.0;

    // Yellow
    colors[9] = 1.0;
    colors[10] = 1.0;
    colors[11] = 0.0;

    // Purple
    colors[12] = 0.0;
    colors[13] = 1.0;
    colors[14] = 1.0;

    // Orange
    colors[15] = 1.0;
    colors[16] = 0.0;
    colors[17] = 1.0; 

    // Pink
    colors[18] = 1.0;
    colors[19] = 0.2;
    colors[20] = 0.7; 

    srand(time(NULL));
    int cColor = 150 + (rand()%105);
    for(int x = 7; x < 100; x++)
    {
        cColor = (cColor + (rand()%255))%255; 
        colors[(x*3) + 0] = ((float)cColor)/255;
        colors[(x*3) + 1] = ((float)((cColor + rand())%255))/255;
        colors[(x*3) + 2] = ((float)((cColor + rand())%255))/255;
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
                inDrag = true;
                xInit = x;
                yInit = y;
                xCur = x;
                yCur = y;
                break;
            case GLUT_UP:
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
                inDrag = true;
                xInit = x;
                yInit = y;
                xCur = x;
                yCur = y;
                break;
            case GLUT_UP:
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

    // Updated mouse position display values
    mousePassiveMotionHandler(x, y);

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
                if((right - yD) - (left + yD) < maxZoomSize) // Max Zoom Limit
                {             
                    left += yD;
                    right -= yD;
                    bottom += yD;
                    top -= yD;
                }
            }
            else if(yD > 0) // Zoom in
            {
                if((right - yD) - (left + yD) > minZoomSize) // Min Zoom limit
                {
                    left += yD;
                    right -= yD;
                    bottom += yD;
                    top -= yD;
                }
            }
            break;
    }
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);

    // Size (width or height) of draw area
    float size = right - left;

    // Sets the robots edge count detail
    robotEdgeCount = (int)(cutOffRange/size) + 3;
    if(robotEdgeCount > 20){robotEdgeCount = 20;}

    // Sets the size of the puck
    puckSize = 8.0/(((size/(float)cutOffRange)*19) + 1);
}

void mousePassiveMotionHandler(int x, int y)
{
    float windowSize = drawServerRef->getWindowSize();

    // Sets the display values for the true position of the mouse
    xTrue = left + ((float)x/windowSize)*(float)(right - left);
    yTrue = top + ((float)y/windowSize)*(float)(bottom - top);
}

void keyEventHandler(unsigned char key, int x, int y)
{
    switch(tolower(key))
    {
        case 'r':
            drawRobot = !drawRobot;
            break;
        case 'f':
            drawFOV = !drawFOV;
            break;
        case 'p':
            drawPuck = !drawPuck;
            break;
        case 't':
            drawTeam = !drawTeam;
            break;
        case 'b':
            drawBoundary = !drawBoundary;
            break;
    }

    printf("Key Pressed: %c \n", key);
}

void initGraphics(int argc, char **argv)
{    
    unsigned int lWindowSize = drawServerRef->getWindowSize();
    
    glutInit(&argc, argv);
    glutInitWindowSize(lWindowSize, lWindowSize);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow(argv[0]);
    glClearColor(0.0, 0.0, 0.0, 1);
    glutDisplayFunc(displayFunc);
    glutIdleFunc(idleFunc);
    glutMouseFunc(mouseClickHandler);
    glutMotionFunc(mouseMotionHandler);
    glutPassiveMotionFunc(mousePassiveMotionHandler);
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

    // Initialize look up arrays
    initializePositionLookupArrays(5, 360, xRobotVals, yRobotVals);
    initializePositionLookupArrays(100, 360, xSensVals, ySensVals);

    // Initialize team color array
    initializeColorArray(colorArray);

    pthread_t thread1;  
	int ret = pthread_create(&thread1, NULL, listener_function, NULL);
	if(ret != 0)
	{
	    perror("pthread failed: can't start client listener.\n");
	} 
    
    glutMainLoop();
}

