/* 
 * File:   main.cpp
 * Author: cla18
 *
 * Created on March 6, 2011, 11:22 PM
 */

#include <stdlib.h>
#include "DrawClient.h"
#include "DrawServer.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    Network::DrawClient * dCli = new Network::DrawClient();
    dCli->update();

    Network::DrawServer * dServer = new Network::DrawServer();
    dServer->draw();

    return (EXIT_SUCCESS);
}

