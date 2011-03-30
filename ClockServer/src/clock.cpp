#include <stdio.h>
#include "ClockServer.h"
#include <stdlib.h>
#include "ClockParser.h"
#include <errno.h>

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
				printf("Prepairing to use port: %s\n", l_port);
				break;
			}
			case('c'):
			{
				l_numClients = atoi(optarg);
				printf("Prepairing to wait on %i clients\n", l_numClients);
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

	l_serv->init(l_port, l_numClients+1); // this +1 is magic for the controller client
	l_serv->start();
	
	return 0;
}
