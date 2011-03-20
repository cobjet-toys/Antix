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
	Network::RobotClient * l_robot = (Network::RobotClient *)args;
	
	if (l_robot == NULL) return -2;
		
	DEBUGPRINT("Entering handler\n");
	if (!commands.empty()) // grid parser
	{
		std::string command = commands.at(0);
		if ( command == "GRID" && commands.size() >= 4)
		{
			int id = atoi(commands.at(1).c_str());
			const char * host = commands.at(2).c_str();
			const char * port = commands.at(3).c_str();
			printf("PARSED: GRID ID %i with IP %s and PORT %s\n", id, host, port);
			return l_robot->initGrid(host,port, id);
		}
		if ( command == "CLOCK" && commands.size() >= 3)
		{	
			const char * host = commands.at(1).c_str();
			const char * port = commands.at(2).c_str();
			printf("PARSED: CLOCK with IP %s and PORT %s\n", host, port);
			return l_robot->initClock(host, port);
		}
        if ( command == "INIT_GRID" && commands.size() >= 2)
		{	
			unsigned int id = atoi(commands.at(1).c_str());
			printf("PARSED: INIT_GRID with ID %d\n", id);
			return l_robot->handleNewGrid(id);
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
