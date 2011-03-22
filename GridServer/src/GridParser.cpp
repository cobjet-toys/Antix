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
	DEBUGPRINT("Entering handler\n");
	if (!commands.empty()) // grid parser
	{
		std::string command = commands.at(0);
		
		if ( command == "NUMBER_OF_TEAMS" && commands.size() >= 2)
		{
			int teamsNum = atoi(commands.at(1).c_str());
			printf("PARSED: Number of teams: %i\n", teamsNum);
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
			printf("PARSED: Robots Per Team: %i\n", robotPerTeam);
			l_gridServer->setRobotsPerTeam(robotPerTeam);
			if (robotPerTeam > 0 )
			{
				m_hasRobotsPerTeam = true;
				return 0;
			}
			return -1;
		}
		if (command == "ID_RANGE" && commands.size() >= 3)
		{
			int from= atoi(commands.at(1).c_str());
			int to=  atoi(commands.at(2).c_str());
			printf("PARSED: ID Range from %i TO %i\n",from ,to);
			l_gridServer->setIdRange(from, to);
			if (from >= 0 && to >= 0 )
			{
				m_hasIdRange = true;
				return 0;
			}
			return -1;			
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

int GridParser::verify()
{
	DEBUGPRINT(" hasrange=%i hasnumteams=%i hasperteam=%i\n", (int)m_hasIdRange, (int)m_hasNumTeams, (int)m_hasRobotsPerTeam);
	return (m_hasIdRange && m_hasNumTeams && m_hasRobotsPerTeam)?0:-1;
}