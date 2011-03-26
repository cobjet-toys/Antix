#include "DrawerParser.h"

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
		
	if (!commands.empty())
	{
		std::string command = commands.at(0);
		if ( command == "GRID" && commands.size() >= 4)
		{
			int id = atoi(commands.at(1).c_str());
			const char * host = commands.at(2).c_str();
			const char * port = commands.at(3).c_str();
			DEBUGPRINT("PARSED: GRID ID %i with IP %s and PORT %s\n", id, host, port);
			return l_grid->initGrid(host, port, id);
		}
		else if (command == "WINDOW_SIZE")
		{
			int size = atoi(commands.at(1).c_str());
			DEBUGPRINT("PARSED: WINDOW_SIZE %i\n", size);
			l_grid->m_windowSize = size;
			return l_grid->m_windowSize == size;
		}
		else if (command == "WORLD_SIZE")
		{
			int size = atoi(commands.at(1).c_str());
			DEBUGPRINT("PARSED: WORLD_SIZE %i\n", size);
			l_grid->m_worldSize = size;
			return l_grid->m_worldSize == size;
		}
		else if (command == "HOME_RADIUS")
		{
			float radius = strtof(commands.at(1).c_str(), NULL);
			DEBUGPRINT("PARSED: HOME_RADIUSE %f\n", radius);
			l_grid->m_homeRadius = radius;
			return l_grid->m_homeRadius == radius;
		}
		else if (command == "ENABLE_FOV")
		{
			bool enable = strcasecmp(commands.at(1).c_str(), "T");
			DEBUGPRINT("PARSED: ENABLE_FOV %i\n", (int)enable);
			l_grid->m_FOVEnabled = enable;
			return l_grid->m_FOVEnabled == enable;
		}
		else if (command == "FOV_ANGLE")
		{
			float angle = strtof(commands.at(1).c_str(), NULL);
			DEBUGPRINT("PARSED: FOV_ANGLE %f\n", angle);
			l_grid->m_FOVAngle = angle;
			return l_grid->m_FOVAngle == angle;
		}
		else if (command == "FOV_RANGE")
		{
			float range = strtof(commands.at(1).c_str(), NULL);
			DEBUGPRINT("PARSED: FOV_RANGE %f\n", range);
			l_grid->m_FOVRange = range;
			return l_grid->m_FOVRange == range;
		}
		else if (command == "#")
		{
			DEBUGPRINT("Skipped Comment\n");
			return 0;
		}
		printf("WARNING: command %s not found or malformed\n", command.c_str());
		return 0;
	}
	return -1;
}
