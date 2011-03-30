#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "ControllerClient.h"
#include "ControllerParser.h"

int main(int argc, char** argv)
{
    setbuf(stdout, NULL);

    if (argc < 3)
    {
        printf("Usage: ./controller -f <init_file>\n");
        return -1;
    }
    
    int l_res = 0;
    char * l_filename = NULL;
    
    while( (l_res = getopt(argc, argv, "f:")) != -1)
    {
    	switch(l_res)
    	{
			case('f'):
			{
				l_filename = optarg;
				DEBUGPRINT("Prepairing to load init file: %s\n", l_filename);
				break;
			}
			case('?'):
			{
				printf("Invalid paramater provided -%i\n", optopt);
        		printf("Usage: ./controller -f <init_file>\n");
				break;
			}
			default:
			{
				abort();
				break;
			}    		
    	
    	}
    
    }

    Network::ControllerClient *  l_CClient = new Network::ControllerClient();
	ControllerParser cntlParser;
	
    if (l_CClient->init() < 0)
	{
		DEBUGPRINT("Failed to init the ControllerClient\n");
		return -1;
	}

	l_res = 0;
	
	DEBUGPRINT("About to readfile\n");
	if ((l_res = cntlParser.readFile(l_filename, (void *)l_CClient )) == ENOENT) 
	{
		printf("Error with parsing file: %s\n", l_filename);
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
