#include "GridParser.h"
#include <iostream>
#include "Config.h"
#include "GridServer.h"
#include <stdlib.h>

GridParser::GridParser(): FileParser()
{
}

GridParser::~GridParser()
{	
	
}

int GridParser::handler(std::vector<std::string> commands, void *args)
{
	Network::GridServer * l_gridServer = (Network::GridServer *) args;
	DEBUGPRINT("Entering handler\n");
	if (!commands.empty()) // grid parser
	{
		std::string command = commands.at(0);
		
		if ( command == "GRID_ID" && commands.size() >= 2)
		{
			int id = atoi(commands.at(1).c_str());
			printf("PARSED: Grid ID Number: %i\n", id);
			l_gridServer->setId(id);
			return (id >= 0)?0:-1;
		}
		if ( command == "NUMBER_OF_TEAMS" && commands.size() >= 2)
		{
			int teamsNum = atoi(commands.at(1).c_str());
			printf("PARSED: Number of teams: %i\n", teamsNum);
			l_gridServer->setTeams(teamsNum);
			return (teamsNum > 0)?0:-1;
		}
		if (command == "ROBOTS_PER_TEAM" && commands.size() >= 2)
		{
			int robotPerTeam = atoi(commands.at(1).c_str());
			printf("PARSED: Robots Per Team: %i\n", robotPerTeam);
			l_gridServer->setRobotsPerTeam(robotPerTeam);
			return (robotPerTeam>0)?0:-1;
		}
		if (command == "ID_RANGE" && commands.size() >= 3)
		{
			int from= atoi(commands.at(1).c_str());
			int to=  atoi(commands.at(2).c_str());
			printf("PARSED: ID Range from %i TO %i\n",from ,to);
			l_gridServer->setIdRange(from, to);
			return (from >= 0 && to >= 0)?0:-1;
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