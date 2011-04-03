#include "DrawerClient.h"

#include <math.h>

#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif

void * listener_function(void* args);
void initGraphics(int argc, char **argv);
void idleFunc();
void timerFunc(int dummy);
void displayFunc();
void drawPucks();
void drawTeams();
void drawRobots(int edgePoints);
void drawText(char* text, float x, float y, bool absolute);
void drawTest(int robotCount, int edgePoints, float viewAngle, int viewEdgePoints);
//void drawRobot(Game::Robot* robot, const GUI::Color* color);
//void drawHome(Game::Home* home, const GUI::Color* color);
void mouseClickHandler(int button, int state, int x, int y);
void mouseMotionHandler(int x, int y);
void keyEventHandler(unsigned char key, int x, int y);
void GlDrawCircle(double x, double y, double r, double count);
void initializePositionLookupArrays(double radius, GLfloat *xVals, GLfloat *yVals);
void initializeColorArray(int *colors);

static int drawCount = 0;

// Framerate Variables
static int framerate = 0, currentFCount = 0;
static double timeIncr = 0.0;
static unsigned int currT = 0, prevT = 0;

// Static DrawServer reference pointer
static Network::DrawServer* drawServerRef = Network::DrawServer::getInstance();

/////////////////////////////////////////
// --- Required Graphics Variables --- //
/////////////////////////////////////////

// Values for mouse drag/zoom options
static int actionType = 0;
static bool inDrag = false;
static float xCur = -1, yCur = -1, xInit = -1, yInit = -1, xEnd = -1, yEnd = -1;
static float zoomLevel = 1.0;

// Variables for Draw options
static bool drawRobot = true, drawFOV = false, drawPuck = true, drawTeam = true;

// Viewport rect positioning values
static float left = 0, top = 0, right = 0, bottom = 0;
static float maxZoomSize = 20000, minZoomSize = 10;

// Lookup arrays for position values based on angle
static GLfloat xRobotVals[360];
static GLfloat yRobotVals[360];

static GLfloat xSensVals[360];
static GLfloat ySensVals[360];

// Variables for drawing detail
static int cutOffRange = 8500;
static int robotEdgeCount = 20;
static GLfloat puckSize = 4.0;

// Array for team colours
static GLfloat colorArray[300];

// TEMPORARY TESTING VARIABLES //
static int angleOffset = 0;
