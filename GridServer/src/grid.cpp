#include "GridServer.h"
#include "GridParser.h"
#include <errno.h>
#include "Config.h"
#include <unistd.h>
#include <time.h>

int main(int argc, char ** argv)
{
    //Seed random number generator
    srand((unsigned)time(0));
    
    setbuf(stdout, NULL);

    Network::GridServer *l_grid = new Network::GridServer();

    #ifdef GRIDTEST
    l_grid->initGridGame();
    #endif

	if (argc < 5)
	{
		printf("Usage: ./grid.bin -p <port> -f <init_file>\n");
		return -1;
	}
	
	
	int l_res = 0;
	opterr = 0;
	char * l_bindPort = NULL;
	char * l_filename = NULL;
	
	while( (l_res = getopt(argc, argv, "p:f:")) != -1)
	{
		switch(l_res)
		{
			case('f'):
			{
				l_filename = optarg;
				DEBUGPRINT("GRID_SERVER STATUS:\t Prepairing to load init file: %s\n", l_filename);
				break;
			}
			case('p'):
			{
				l_bindPort = optarg;
				DEBUGPRINT("GRID_SERVER STATUS:\t Prepairing to use port %s\n", l_bindPort);
				break;
			}
			case('?'):
			{
				printf("GRID_SERVER STATUS:\t Invalid paramater provided -%i\n", optopt);
				printf("Usage: ./grid.bin -p <port> -f <init_file>\n");
				break;
			}
			default:
			{
				abort();
				break;
			}
		}
	}
	
	

	
	GridParser l_parser;
	
	l_res = 0;
	
	if ((l_res = l_parser.readFile(l_filename, (void *)l_grid )) == ENOENT) 
	{
		LOGPRINT("GRID_PARSER FAIL:\tError with parsing file: %s\n", l_filename);
		return -1;
	}
	
	l_res = 0;
	
	if (l_res < 0)
	{
		LOGPRINT("GRID_PARSER FAIL:\tFailed to parse file\n");
		return -1;
	}
	
	l_res = 0;
	
	if ((l_res = l_parser.verify()) < 0)
	{
		LOGPRINT("GRID_PARSER FAIL:\tGame not initialized\n");
		return -1;
	} 
	
	
	if (l_grid->init(l_bindPort) < 0) 
	{
		LOGPRINT("GRID FAIL:\t Cannot init port\n");
		return -1;
	}
	
	if (l_grid->start() < 0 )
	{
		LOGPRINT("GRID FAIL:\t Server failed, did not exit cleanly.\n");
		return -1;
	}
	
	return 0;
}
