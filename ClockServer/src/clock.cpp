#include <stdio.h>
#include "ClockServer.h"
#include <stdlib.h>


int main(int argc, char ** argv)
{
    setbuf(stdout, NULL);
	if (argc < 3) 
	{
		printf("Usage: ./clock.bin <clock_port> <num_clients>\n");
		return -1;
	}
	Network::ClockServer serv;
	serv.init(argv[1], atoi(argv[2]));
	serv.start();
	
	return 0;
}
