#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif

#include "DrawServer.h"
#include <math.h>

using namespace Network;

DrawServer::DrawServer():Server()
{
    int argc=0;
    char ** argv = NULL;

    initGraphics(argc,argv);
}

int DrawServer::handler(int fd)
{
    TcpConnection * l_curConnection = m_Clients[fd];
    if (l_curConnection == NULL) return -1; //no socket descriptor

    /* Get message header */
    Msg_header l_msgHeader;
    unsigned char * msg = new unsigned char[20];
    uint16_t sender=-1, msgType =-1;

    memset(msg, 0, l_msgHeader.size);
    if (l_curConnection->recv(msg, (int)l_msgHeader.size) != 0) return -1; //recv failed

    printf("Reciving message from client\n");
    unpack(msg, "hh", &sender, &msgType);
    printf("%u, %u\n", sender, msgType);
    if (sender == -1 || msgType == -1) return -1; // bad messages


    switch (sender)
    {
        case SENDER_GRIDSERVER:
            break;

        case SENDER_CLIENT:
            Msg_InitHome homeMsg;
            memset(msg, 0, homeMsg.size);

            /* Receive Home details */
            printf("Reciving a Home from a Client\n");
            if (l_curConnection->recv(msg, homeMsg.size) != 0) return -1; //recv failed

            float homeRadius, homeMaxWorldSize, homeX, homeY, colR, colG, colB;
            unpack(msg, "fffffff",
                    &homeRadius, &homeMaxWorldSize, &homeX, &homeY, &colR, &colG, &colB);
            /*
            Game::Home* home = new Game::Home(new Math::Position(homeX, homeY));
            if (home != NULL)
            {
                printf("Added: Home<%f,%f>\n", homeX, homeY);
                m_Homes->push_back(home);
            }
             */

            /* Send ACK */
            l_msgHeader.sender = SENDER_DRAWER;
            l_msgHeader.message =  MSG_ACK;
            unsigned char cliBuf[l_msgHeader.size];

            pack(cliBuf, "hh", l_msgHeader.sender, l_msgHeader.message);
            if (l_curConnection->send(cliBuf, l_msgHeader.size) < 0)
            {
                printf("Unable to send Home ACK to Client\n");
            }    
            break;

        case SENDER_CONTROLLER:
            Msg_InitDrawer initMsg;
            memset(msg, 0, initMsg.size);

            /* Receive Home details */
            printf("Reciving a init details from Controller\n");
            if (l_curConnection->recv(msg, initMsg.size) != 0) return -1; //recv failed

            unpack(msg, "cffff",
                    &m_EnableFOV, &m_FoVAngle, &m_FoVRange, &m_WorldSize, &m_WindowSize);
            printf("Set: EnableFOV=%c,FoVAngle=%f,FoVRange=%f,WorldSize=%f,WindowSize=%f\n",
                    m_EnableFOV, m_FoVAngle, m_FoVRange, m_WorldSize, m_WindowSize);

            /* Send ACK */
            l_msgHeader.sender = SENDER_DRAWER;
            l_msgHeader.message =  MSG_ACK;
            unsigned char ctrlBuf[l_msgHeader.size];

            pack(ctrlBuf, "hh", l_msgHeader.sender, l_msgHeader.message);
            if (l_curConnection->send(ctrlBuf, l_msgHeader.size) < 0)
            {
                printf("Unable to send Home ACK to Client\n");
            }
            break;

        default:
            printf("ERROR: no matching msg handler for UNKNOWN\n");
            return 0;
            break;
    }
    delete msg;
    return 0;
}


void idleFunc()
{
    //Game::Robotix::getInstance()->update();
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

    /* Draw Pucks */
    /* Draw Homes */
    /* Draw Robots */

    /*
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
    */

    glutSwapBuffers();
    glutTimerFunc( 20, timerFunc, 0);
}

void drawPucks()
{
    glColor3f( 1, 1, 1);

    /*
    glBegin(GL_POINTS);

    for (Game::PuckIter it = Game::Robotix::getInstance()->getFirstPuck();
            it != Game::Robotix::getInstance()->getLastPuck(); it++)
    {
        glVertex2f( (*it)->getPosition()->getX(), (*it)->getPosition()->getY() );
    }
    glEnd();
     */

}

void drawTeams()
{
    /*
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
    */
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
        if (m_EnableFOV)
        {
            glBegin(GL_LINE_LOOP);
            glVertex2f(0,0);
            float l_Right = -m_FoVAngle/2.0;
            float l_Left = +m_FoVAngle/2.0;
            float l_Incr = m_FoVAngle/32.0;

            for(float i = l_Right; i < l_Left; i+=l_Incr)
                glVertex2f(cos(i)*m_FoVRange, sin(i)*m_FoVRange);
            glVertex2f(cos(l_Left)*m_FoVRange, sin(l_Left)*m_FoVRange);
            glEnd();

        }
    }

    glPopMatrix();
}
*/

// utility
void GlDrawCircle( double x, double y, double r, double count )
{
	glBegin(GL_LINE_LOOP);
	for( float a=0; a<(M_PI*2.0); a+=M_PI/count )
		glVertex2f( x + sin(a) * r, y + cos(a) * r );
	glEnd();
}


void DrawServer::drawHome(float xloc, float yloc, float radius, float colR, float colG, float colB)
{
    float worldSize = m_WorldSize;

    glColor3f(colR, colG, colB);
    GlDrawCircle( xloc, yloc, radius, 16 );
    GlDrawCircle( xloc+worldSize, yloc, radius, 16 );
    GlDrawCircle( xloc-worldSize, yloc, radius, 16 );
    GlDrawCircle( xloc, yloc+worldSize, radius, 16 );
    GlDrawCircle( xloc, yloc-worldSize, radius, 16 );
}


void DrawServer::initGraphics(int argc, char **argv)
{
    float worldSize = m_WorldSize;
    float windowSize = m_WindowSize;
    
    glutInit(&argc, argv);
    glutInitWindowSize( windowSize, windowSize );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutCreateWindow( argv[0] );
    glClearColor( 0.1, 0.1, 0.1, 1 );
    glutDisplayFunc( displayFunc );
    glutIdleFunc( idleFunc );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0,1,0,1 );

    glScalef( 1.0/worldSize, 1.0/worldSize, 1 );
	glPointSize( 4.0 );
    glutMainLoop();
}