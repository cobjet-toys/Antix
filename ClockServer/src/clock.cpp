#include <stdio.h>
#include "ClockServer.h"
#include <stdlib.h>


int main(int argc, char ** argv)
{
	if (argc < 3) 
	{
		printf("Invalid arguments: specify a port\n");
		return -1;
	}
	Network::ClockServer serv;
	serv.init(argv[1], atoi(argv[2]));
	serv.start();
	
	return 0;
}