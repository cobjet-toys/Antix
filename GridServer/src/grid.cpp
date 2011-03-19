#include "GridServer.h"
#include "GridParser.h"
#include <errno.h>

int main(int argc, char ** argv)
{
	Network::GridServer *l_grid = new Network::GridServer();
	if (argc < 3) 
	{
		printf("Usage: ./grid.bin <port> <init_file>\n");
		return -1;
	}
	
	GridParser l_parser;
	
	int l_res = 0;
	
	if ((l_res = l_parser.readFile(argv[2], (void *)l_grid )) == ENOENT) 
	{
		printf("Error with parsing file: %s\n", argv[2]);
		return -1;
	}
	if (l_res < 0)
	{
		printf("Failed to parse file\n");
		return -1;
	}
	
	if (l_grid->init(argv[1]) < 0) 
	{
		return -1;
	}
	
	l_grid->start();
	
	return 0;
}