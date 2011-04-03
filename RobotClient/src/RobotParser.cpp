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
        if ( command == "ROBOT_FOV" && commands.size() >= 2)
		{	
			float fov = atof(commands.at(1).c_str());
			printf("PARSED: Robot FOV %f\n", fov);
			l_robot->setFOV(fov);
			return 0;
		}
        if ( command == "ROBOT_RADIUS" && commands.size() >= 2)
		{	
			float radius = atof(commands.at(1).c_str());
			printf("PARSED: ROBOT RADIUS %f\n", radius);
			l_robot->setRadius(radius);
			return 0;			
		}
        if ( command == "SENSOR_RANGE" && commands.size() >= 2)
		{	
			float sensorRange = atof(commands.at(1).c_str());
			printf("PARSED: SENSOR_RANGE %f\n", sensorRange);
			l_robot->setSensorRange(sensorRange);
			return 0;			
		}
        if ( command == "PICKUP_RANGE" && commands.size() >= 2)
		{	
			float pickupRange = atof(commands.at(1).c_str());
			printf("PARSED: PICKUP_RANGE  %f\n", pickupRange);
			l_robot->setPickupRange(pickupRange);
			return 0;			
		}
        if ( command == "WORLD_SIZE" && commands.size() >= 2)
		{	
			float worldSize = atof(commands.at(1).c_str());
			printf("PARSED: WORLD_SIZE  %f\n", worldSize);
			l_robot->setWorldSize(worldSize);
			return 0;			
		}
        if ( command == "HOME_RADIUS" && commands.size() >= 2)
		{	
			float homeRadius = atof(commands.at(1).c_str());
			printf("PARSED: PICKUP_RANGE  %f\n", homeRadius);
			l_robot->setHomeRadius(homeRadius);
			return 0;			
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
