#include "DrawServer.h"

int main(int argc, char ** argv)
{
	Network::DrawServer drawer;
	if (argc < 2)
	{
		perror("Please specify a port for the server");
		return -1;
	}
	if (drawer.init(argv[1]) < 0) return -1;

	drawer.start();

	return 0;
}