#include "DrawServer.h"

#include <math.h>

void * listener_function(void* args);
void initGraphics(int argc, char **argv);
void idleFunc();
void timerFunc(int dummy);
void displayFunc();
void drawPucks();
void drawTeams();
void drawRobots();
void drawText(char* text, float x, float y);
void drawTest();
//void drawRobot(Game::Robot* robot, const GUI::Color* color);
//void drawHome(Game::Home* home, const GUI::Color* color);
void mousFunc(int button, int state, int x, int y);
void GlDrawCircle( double x, double y, double r, double count );
static int drawCount = 0;

// Values for mouse drag/zoom options
static int actionType = 0;

static bool inDrag = false;
static int xCur = -1, yCur = -1, xInit = -1, yInit = -1, xEnd = -1, yEnd = -1;

static float zoomLevel = 1.0;

// Viewport rect positioning values
static int left = 0, top = 0, right = 0, bottom = 0;
