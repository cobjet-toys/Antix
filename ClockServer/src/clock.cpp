#include <stdio.h>
#include "ClockServer.h"
#include <stdlib.h>
#include "ClockParser.h"
#include <errno.h>
#include "Config.h"

int main(int argc, char ** argv)
{
    setbuf(stdout, NULL);
	if (argc < 5) 
	{
		printf("Usage: ./clock.bin -p <clock_port> -c <num_clients>\n");
		return -1;
	}

	char * l_port = NULL;
	int l_numClients = 0;
	int l_res = 0;
	opterr = 0;
	
	while( (l_res = getopt(argc, argv, "p:c:") ) != -1)
	{
		switch(l_res)
		{
			case('p'):
			{
				l_port = optarg;
				DEBUGPRINT("Prepairing to use port: %s\n", l_port);
				break;
			}
			case('c'):
			{
				l_numClients = atoi(optarg);
				DEBUGPRINT("Prepairing to wait on %i clients\n", l_numClients);
				break;
			}			
			case('?'):
			{
				printf("Usage: ./clock.bin -p <clock_port> -c <num_clients>\n");
				return -1;
			}
			default:
				abort();
			
		}
	
	}	

	Network::ClockServer *l_serv = new Network::ClockServer();

	if (l_serv->init(l_port, l_numClients+1) < 0) // this +1 is magic for the controller client
	{
		ERRORPRINT("CLOCK_SERVER ERROR:\t Failed to init CLOCK\n");
		return -1;
	}
	
	if (l_serv->start() < 0)
	{
		ERRORPRINT("CLOCK_SERVER ERROR:\t CLOCK_SERVER failure. Exiting\n");
		return -1;		
	}
	
	return 0;
}
