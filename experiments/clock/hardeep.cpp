#include "GridServer.h"

int main(int argc, char ** argv)
{
	Network::GridServer hardserve;
	
	hardserve.init(argv[1]);
	hardserve.start();
	
	return 0;
}