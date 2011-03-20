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
