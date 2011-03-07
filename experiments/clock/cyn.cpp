/* 
 * File:   cyn.cpp
 * Author: cla18
 *
 * Created on March 6, 2011, 5:36 PM
 */

#include <stdlib.h>
#include "DrawClient.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Usage: dstubs.bin <drawer IP address> <drawer port> [<total clients> <total robots>]\n");
    }

    int numClients = 1;
    int numRobots = 30000;
    if (argc > 4)
        numRobots = atoi(argv[4]);
    if (argc > 3)
        numClients = atoi(argv[3]);

    for (int i=0; i<numClients; i++)
    {
        /* Create draw client, create connection with drawer */
        Network::DrawClient c;
        int g = c.init();
        printf("g=%i \n", g);

        int i = c.initConnection(argv[1], argv[2]);
        if (i < 0)
        {
            printf("init failed\n.");
            return (EXIT_FAILURE);
        }

        printf("Socket connection to drawer: %i\n", i);
        c.setNumOfRobots( numRobots/numClients );
        c.start();
    }

    return (EXIT_SUCCESS);
}

