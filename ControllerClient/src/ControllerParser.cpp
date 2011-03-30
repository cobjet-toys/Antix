#include "ControllerParser.h"
#include <iostream>
#include "Config.h"
#include "ControllerClient.h"
#include <stdlib.h>
#include <unistd.h>

ControllerParser::ControllerParser(): FileParser()
{
}

ControllerParser::~ControllerParser()
{	
	
}

int ControllerParser::handler(std::vector<std::string> commands, void *args)
{
	Network::ControllerClient * l_cntlClient = (Network::ControllerClient *) args;
	DEBUGPRINT("Entering handlers\n");

	if (!commands.empty()) // grid parser
	{
		std::string command = commands.at(0);
		
		if ( command == "GRID" && commands.size() >= 3)
		{
			const char * ipAddr = commands.at(1).c_str();
			const char * port = commands.at(2).c_str();
			printf("PARSED: Grid: IP=%s PORT=%s\n", ipAddr, port);
			if (l_cntlClient->initGrid(ipAddr, port) < 0) return -1;
			return (ipAddr != NULL && port != NULL)?0:-1;
		}

		if ( command == "CLOCK" && commands.size() >= 3)
		{
			const char * ipAddr = commands.at(1).c_str();
			const char * port = commands.at(2).c_str();
			printf("PARSED: Clock: IP=%s PORT=%s\n", ipAddr, port);
			l_cntlClient->initClock(ipAddr, port);
			return (ipAddr != NULL && port != NULL)?0:-1;
		}		
		if ( command == "HOME_RADIUS" && commands.size() >= 2)
		{
			float radius = atof(commands.at(1).c_str());
			printf("PARSED: Home radius: %f\n", radius);
			l_cntlClient->setHomeRadius(radius);
			return true;
		}	
		if ( command == "WORLD_SIZE" && commands.size() >= 2)
		{
			float worldSize = atof(commands.at(1).c_str());
			printf("PARSED: World size: %f\n", worldSize);
			l_cntlClient->setWorldSize(worldSize);
			return true;
		}
		if ( command == "NUM_GRIDS" && commands.size() >= 2)
		{
			int gridSize = atoi(commands.at(1).c_str());
			printf("PARSED: Number of grids: %i\n", gridSize);
			l_cntlClient->numGrids(gridSize);
			return true;
		}	
		if ( command == "PUCK_TOTAL" && commands.size() >= 2)
		{
			int pucks = atoi(commands.at(1).c_str());
			printf("PARSED: Number of pucks: %i\n", pucks);
			l_cntlClient->numPucksTotal(pucks);
			return true;
		}								
		if (command == "#")
		{
			DEBUGPRINT("Skipped Comment\n");
			return 0;
		}
		printf("WARNING: command %s not found or malformed\n", command.c_str());
		return 0;
	}
	return -1;
}
