#include "Team.h"
#include <math.h>
#include "MathAux.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Game;

Team::Team(Math::Position *pos, unsigned int id):GameObject(pos, id)
{
}

Team::~Team()
{
}

float Team::getX()
{

	return (*getPosition()).getX();

}

float Team::getY()
{

	return (*getPosition()).getY();

}


