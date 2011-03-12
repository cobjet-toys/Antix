/* 
 * File:   main.cpp
 * Author: cla18
 *
 * Created on March 6, 2011, 11:22 PM
 */

#include <stdlib.h>
#include "DrawServer.h"
#include "dutils.h"
#include "Gui.h"

/*
 * Usage: ./DrawServer <window_size> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ]
 */
int main(int argc, char** argv)
{
    /*
    if (argc < 5)
    {
        printf("Usage: ./DrawServer <window_size> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ] ");
        return (EXIT_FAILURE);
    }

    if (atoi(argv[4]) != 0 && argc < 7)
    {
        printf("Usage: ./DrawServer <window_size> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ] ");
        return (EXIT_FAILURE);
    }
     * */
    
    Network::DrawServer::getInstance()->init(argc, argv);

    //for(int i=0; i<3; i++)
        //Network::DrawServer::getInstance()->update();

    initGraphics(argc, argv);
    
    return (EXIT_SUCCESS);
}

