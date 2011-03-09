#include <stdio.h>
#include "ClockServer.h"


int main(int argc, char ** argv)
{
	if (argc < 2) 
	{
		printf("Invalid arguments: specify a port\n");
		return -1;
	}
	Network::ClockServer serv;
	serv.init(argv[1]);
	serv.start();
	
	return 0;
}