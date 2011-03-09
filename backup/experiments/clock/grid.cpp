#include "GridServer.h"

int main(int argc, char ** argv)
{
	Network::GridServer grid;
	if (argc < 2) 
	{
		perror("Please specify a port for the server");
		return -1;
	}
	if (grid.init(argv[1]) < 0) 
	{
		return -1;
	}
	
	grid.start();
	
	return 0;
}