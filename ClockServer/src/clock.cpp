#include <stdio.h>
#include "ClockServer.h"
#include <stdlib.h>
#include "ClockParser.h"
#include <errno.h>

int main(int argc, char ** argv)
{
	if (argc < 4) 
	{
		printf("Usage: ./clock.bin <clock_port> <num_clients> <init_file>\n");
		return -1;
	}

	Network::ClockServer *l_serv = new Network::ClockServer();
	ClockParser l_parser;
	
	int l_res = 0;
	
	if ((l_res = l_parser.readFile(argv[3], (void *)l_serv )) == ENOENT) 
	{
		printf("Error with parsing file: %s\n", argv[3]);
		return -1;
	}
	if (l_res < 0)
	{
		printf("Failed to parse file\n");
		return -1;
	}

	l_serv->init(argv[1], atoi(argv[2])+1);
	l_serv->start();
	
	return 0;
}
