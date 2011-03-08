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
 * 
 */
int main(int argc, char** argv)
{
    Network::DrawServer::getInstance()->init(argc, argv);
    Network::DrawServer::getInstance()->update();

    initGraphics(argc, argv);
    
    return (EXIT_SUCCESS);
}

