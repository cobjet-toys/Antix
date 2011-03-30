#include "DrawerParser.h"

DrawerParser::DrawerParser(): FileParser()
{
	
}

DrawerParser::~DrawerParser()
{	
	
}

int DrawerParser::handler(std::vector<std::string> commands, void *args)
{
	Network::DrawServer * l_drawer = (Network::DrawServer *)args;
	
	if (l_drawer == NULL) return -2;
		
	if (!commands.empty())
	{
		std::string command = commands.at(0);
		if ( command == "GRID" && commands.size() >= 3)
		{
			int id = atoi(commands.at(1).c_str());
			const char * host = commands.at(2).c_str();
			const char * port = commands.at(3).c_str();
			DEBUGPRINT("PARSED: GRID ID %i with IP %s and PORT %s\n", id, host, port);
			return l_drawer->initGrid(host, port);
		}
		else if (command == "WINDOW_SIZE")
		{
			int size = atoi(commands.at(1).c_str());
			DEBUGPRINT("PARSED: WINDOW_SIZE %i\n", size);
			l_drawer->setWindowSize(size);
			return l_drawer->getWindowSize() == size;
		}
		else if (command == "WORLD_SIZE")
		{
			int size = atoi(commands.at(1).c_str());
			DEBUGPRINT("PARSED: WORLD_SIZE %i\n", size);
			l_drawer->setWorldSize(size);
			return l_drawer->getWorldSize() == size;
		}
		else if (command == "HOME_RADIUS")
		{
			float radius = strtof(commands.at(1).c_str(), NULL);
			DEBUGPRINT("PARSED: HOME_RADIUS %f\n", radius);
			l_drawer->setHomeRadius(radius);
			return l_drawer->getHomeRadius() == radius;
		}
		else if (command == "TOTAL_NUM_TEAMS")
		{
			int size = atoi(commands.at(1).c_str());
			DEBUGPRINT("PARSED: TOTAL_NUM_TEAMS %i\n", size);
			l_drawer->m_totalNumTeams = size;
			return l_drawer->m_totalNumTeams == size;
		}
		else if (command == "TOTAL_NUM_ROBOTS")
		{
			int size = atoi(commands.at(1).c_str());
			DEBUGPRINT("PARSED: TOTAL_NUM_ROBOTS %i\n", size);
			
			Math::Position *pos = new Math::Position(0.0, 0.0, 0.0);		
			return l_drawer->initRobots(size) == size;
		}
		else if (command == "TOTAL_NUM_PUCKS")
		{
			int size = atoi(commands.at(1).c_str());
			DEBUGPRINT("PARSED: TOTAL_NUM_PUCKS %i\n", size);
			
			Math::Position *pos = new Math::Position(0.0, 0.0, 0.0);
			return l_drawer->initPucks(size) == size;
		}
		else if (command == "ENABLE_FOV")
		{
			bool enable = strcasecmp(commands.at(1).c_str(), "T");
			DEBUGPRINT("PARSED: ENABLE_FOV %i\n", (int)enable);
			l_drawer->setFOVEnabled(enable);
			return l_drawer->getFOVEnabled() == enable;
		}
		else if (command == "FOV_ANGLE")
		{
			float angle = strtof(commands.at(1).c_str(), NULL);
			DEBUGPRINT("PARSED: FOV_ANGLE %f\n", angle);
			l_drawer->setFOVAngle(angle);
			return l_drawer->getFOVAngle() == angle;
		}
		else if (command == "FOV_RANGE")
		{
			float range = strtof(commands.at(1).c_str(), NULL);
			DEBUGPRINT("PARSED: FOV_RANGE %f\n", range);
			l_drawer->setFOVRange(range);
			return l_drawer->getFOVRange() == range;
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
