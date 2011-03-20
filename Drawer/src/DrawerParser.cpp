#include "DrawerParser.h"
#include <iostream>
#include "Config.h"
#include "DrawServer.h"

DrawerParser::DrawerParser(): FileParser()
{
	
}

DrawerParser::~DrawerParser()
{	
	
}

int DrawerParser::handler(std::vector<std::string> commands, void *args)
{
	Network::DrawServer * l_grid = (Network::DrawServer *)args;
	
	if (l_grid == NULL) return -2;
		
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
			return l_grid->initGrid(host, port, id);
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
