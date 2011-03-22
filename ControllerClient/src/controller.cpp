#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "ControllerClient.h"
#include "ControllerParser.h"

int main(int argc, char** argv)
{

    if (argc < 2)
    {
        printf("Usage: ./controller {{FILENAME}}\n");
        return -1;
    }

    Network::ControllerClient *  l_CClient = new Network::ControllerClient();
	ControllerParser cntlParser;
	
    if (l_CClient->init() < 0)
	{
		DEBUGPRINT("Failed to init the ControllerClient\n");
		return -1;
	}

	int l_res = 0;
	
	DEBUGPRINT("About to readfile\n");
	if ((l_res = cntlParser.readFile(argv[1], (void *)l_CClient )) == ENOENT) 
	{
		printf("Error with parsing file: %s\n", argv[1]);
		return -1;

	}
	if (l_res < 0)
	{
		printf("Failed to parse file\n");
		return -1;
	}
	DEBUGPRINT("Done reading files\n");
	
	l_CClient->start();
	
    return 0;
}





















//l_CClient->initNeighbourGrids();
	
    /* TODO Connect to all grid servers.
    l_CClient.initGrid("127.0.0.1", "12345");
    l_CClient.initGrid("127.0.0.1", "12346");
    l_CClient.initGrid("127.0.0.1", "12347");

    //TODO Connect all the grid servers together.
   

    //TODO Connect to all robot clients.
    l_CClient.initRobotClient("127.0.0.1", "12348");
    
    //TODO Connect to clock.
    l_CClient.initClock("127.0.0.1", "12349");

    l_CClient.beginSimulation();
    l_CClient.start();*/