#include "RobotParser.h"
#include <iostream>
#include "Config.h"
#include "RobotClient.h"

RobotParser::RobotParser(): FileParser()
{
	
}

RobotParser::~RobotParser()
{	
	
}

int RobotParser::handler(std::vector<std::string> commands, void *args)
{
	int grid_id = 1; //TODO: This needs to be a uniquily assigned id from Controller for RobotClient to initgrid
	Network::RobotClient * l_robot = (Network::RobotClient *)args;
	
	if (l_robot == NULL) return -2;
		
	DEBUGPRINT("Entering handler\n");
	if (!commands.empty()) // grid parser
	{
		std::string command = commands.at(0);
		if ( command == "GRID" && commands.size() >= 3)
		{
			printf("PARSED: GRID with IP %s and PORT %s\n", commands.at(1).c_str(), commands.at(2).c_str(), grid_id);
			return l_robot->initGrid(commands.at(1).c_str(), commands.at(2).c_str(), grid_id);
		}
		if ( command == "CLOCK" && commands.size() >= 3)
		{
			printf("PARSED: CLOCK with IP %s and PORT %s\n", commands.at(1).c_str(), commands.at(2).c_str());
			return l_robot->initClock(commands.at(1).c_str(), commands.at(2).c_str());
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
