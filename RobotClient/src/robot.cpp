#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "RobotClient.h"
#include "FileUtil.h"

using namespace Antix;

int main(int argc, char** argv)
{

    if (argc < 3)
    {
        printf("Usage: ./robotclient.bin server.info system.config client_num\n");
        return -1;
    }

    const char* server_fn = argv[1];
    const char* config_fn = argv[2];
    const int client_num = lexical_cast<int>(argv[3]);

    ConnectionList grid_servers;
    ConnectionList robot_clients;
    ConnectionPair clock_server;
    ConnectionPair draw_server;
    
    Antix::parseServerFile(server_fn, grid_servers, robot_clients, clock_server, draw_server); 
    cout << grid_servers[0].first << " " << grid_servers[0].second << endl;
    cout << clock_server.first << " " << clock_server.second << endl;
    cout << draw_server.first << endl;
    cout << robot_clients[0].first << endl;

/*
    Network::RobotClient rclient;

    rclient.init();

    //TODO Get grid servers

    for(ConListIterator iter = grid_servers.begin();
        iter != grid_servers.end();
        iter++)
    {
        //rclient.initGrid( (*iter).first, (*iter).second);
    }

    //Connect to the clock
    //rclient.initClock(clock_server.first, clock_server.second);

    //rclient.start();
*/
    return 0;
}
