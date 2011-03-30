#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "RobotParser.h"
#include "RobotClient.h"
#include "Config.h"


int main(int argc, char** argv)
{
    setbuf(stdout, NULL);

    if (argc <3)
    {
        printf("Usage: ./client.bin -f <init_file>\n");
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
        		printf("Usage: ./client.bin -f <init_file>\n");
				break;
			}
			default:
			{
				abort();
				break;
			}    		
    	
    	}
    
    }
    

    Network::RobotClient * l_rclient = new Network::RobotClient;
    l_rclient->init();
	
	RobotParser l_parser;
	
	l_res = 0;

	DEBUGPRINT("About to readfile\n");

	if ((l_res = l_parser.readFile(l_filename, (void *)l_rclient )) == ENOENT) 
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

    l_rclient->start();

    return 0;
}

