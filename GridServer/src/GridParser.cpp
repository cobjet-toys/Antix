#include "GridParser.h"
#include <iostream>
#include "Config.h"
#include "GridServer.h"
#include <stdlib.h>

GridParser::GridParser(): FileParser()
{
	m_hasNumTeams = false;
	m_hasRobotsPerTeam = false;
	m_hasIdRange = false;
}

GridParser::~GridParser()
{	
	
}

int GridParser::handler(std::vector<std::string> commands, void *args)
{
	Network::GridServer * l_gridServer = (Network::GridServer *) args;
	DEBUGPRINT("GRID_PARSER STATUS:\t Entering handler\n");
	if (!commands.empty()) // grid parser
	{
		std::string command = commands.at(0);
		
		if ( command == "NUMBER_OF_TEAMS" && commands.size() >= 2)
		{
			int teamsNum = atoi(commands.at(1).c_str());
			LOGPRINT("GRID_PARSER STATUS:\t PARSED: Number of teams: %i\n", teamsNum);
			l_gridServer->setTeams(teamsNum);
			if (teamsNum > 0 )
			{
				m_hasNumTeams = true;
				return 0;
			}
			return -1;
		}
		if (command == "ROBOTS_PER_TEAM" && commands.size() >= 2)
		{
			int robotPerTeam = atoi(commands.at(1).c_str());
			LOGPRINT("GRID_PARSER STATUS:\t PARSED: Robots Per Team: %i\n", robotPerTeam);
			l_gridServer->setRobotsPerTeam(robotPerTeam);
			if (robotPerTeam > 0 )
			{
				m_hasRobotsPerTeam = true;
				return 0;
			}
			return -1;
		}
		if (command == "#")
		{
			DEBUGPRINT("GRID_PARSER STATUS:\t Skipped Comment\n");
			return 0;
		}
		printf("GRID_PARSER WARNING:\tcommand %s not found or malformed\n", command.c_str());
		return 0;
	}
	return -1;
}

int GridParser::verify()
{
	DEBUGPRINT("GRID_PARSER STATUS:\tVERIFY hasnumteams=%i hasperteam=%i\n", (int)m_hasNumTeams, (int)m_hasRobotsPerTeam);
	return (m_hasNumTeams && m_hasRobotsPerTeam)?0:-1;
}
