#include "ClockParser.h"
#include <iostream>
#include "Config.h"


ClockParser::ClockParser(): FileParser()
{
}

ClockParser::~ClockParser()
{	
	
}

int ClockParser::handler(std::vector<std::string> commands, void *args)
{
	DEBUGPRINT("Entering handler\n");
	if (!commands.empty()) // grid parser
	{
		std::string command = commands.at(0);
		
		
		
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