/* 
 * File:   drawer.cpp
 * Author: cla18
 *
 * Created on March 6, 2011, 11:22 PM
 */

#include <stdlib.h>
#include "DrawServer.h"
#include "Gui.h"

/*
 * Usage: ./DrawServer <window_size> <init_file> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ]
 */
int main(int argc, char** argv)
{
	if (argc < 3) 
	{
		perror("Please specify a port and a config file for the Drawer");
		return (EXIT_FAILURE);
	}
    
    Network::DrawServer * drawer = Network::DrawServer::getInstance();
    drawer->init();
    drawer->initGrid("localhost", "3333", 0);
	
	/*
	DrawerParser l_parser;	
	int l_res = 0;
	DEBUGPRINT("About to readfile\n");
	if ((l_res = l_parser.readFile(argv[1], (void *)drawer )) == ENOENT) 
	{
		printf("Error with parsing file: %s\n", argv[2]);
		return -1;
	}
	if (l_res < 0)
	{
		printf("Failed to parse file\n");
		return -1;
	}
	DEBUGPRINT("Done reading files\n");
	*/

    initGraphics(argc, argv);
    
    return (EXIT_SUCCESS);
}

