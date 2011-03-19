#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "ControllerClient.h"

int main(int argc, char** argv)
{

    /*if (argc <3)
    {
        printf("Specify filename and Port\n");
        return -1;
    }*/

    Network::ControllerClient l_CClient;

    l_CClient.init();

    //TODO Connect to all grid servers.
    l_CClient.initGrid("127.0.0.1", "12345");
    l_CClient.initGrid("127.0.0.1", "12346");
    l_CClient.initGrid("127.0.0.1", "12347");

    //TODO Connect all the grid servers together.
    l_CClient.initNeighbourGrids();

    //TODO Connect to all robot clients.
/*    l_CClient.initRobotClient("127.0.0.1", "12348");
    
    //TODO Connect to clock.
    l_CClient.initClock("127.0.0.1", "12349");

    l_CClient.beginSimulation();*/
    l_CClient.start();
    return 0;
}
