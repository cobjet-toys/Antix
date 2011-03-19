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
 * Usage: ./DrawServer <window_size> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ]
 */
int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		perror("Please specify a port for the server");
		return (EXIT_FAILURE);
	}
	
    /*
    if (argc < 6)
    {
        perror("Usage: ./DrawServer <port number> <window_size> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ] ");
        return (EXIT_FAILURE);
    }

    if (atoi(argv[5]) != 0 && argc < 8)
    {
        perror("Usage: ./DrawServer <port number> <window_size> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ] ");
        return (EXIT_FAILURE);
    }
     * */     
    
    //Network::DrawServer::getInstance()->init(argc, argv);
    int ret = Network::DrawServer::getInstance()->init();
    printf("ret=%d\n", ret);
    Network::DrawServer::getInstance()->initGrid("142.58.35.211", "3333");
    Network::DrawServer::getInstance()->initTeams();

    initGraphics(argc, argv);
    
    return (EXIT_SUCCESS);
}

