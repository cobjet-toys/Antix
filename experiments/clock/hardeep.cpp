#include "Client.h"

int main(int argc, char ** argv)
{
	Network::Client hardbot;
	
	int returnV = hardbot.init(argv[1], argv[2]);
	printf("%i\n", returnV);
	
	return 0;
}