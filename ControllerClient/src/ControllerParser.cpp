#include "ControllerParser.h"
#include <iostream>
#include "Config.h"
#include "ControllerClient.h"
#include <stdlib.h>

ControllerParser::ControllerParser(): FileParser()
{
}

ControllerParser::~ControllerParser()
{	
	
}

int ControllerParser::handler(std::vector<std::string> commands, void *args)
{
	Network::ControllerClient * l_cntlClient = (Network::ControllerClient *) args;
	DEBUGPRINT("Entering handler\n");
	if (!commands.empty()) // grid parser
	{
		std::string command = commands.at(0);
		
		if ( command == "GRID" && commands.size() >= 3)
		{
			const char * ipAddr = commands.at(1).c_str();
			const char * port = commands.at(2).c_str();
			printf("PARSED: Grid: IP=%s PORT=%s\n", ipAddr, port);
			l_cntlClient->initGrid(ipAddr, port);
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