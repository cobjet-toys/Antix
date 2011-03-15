#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "RobotClient.h"
#include "FileUtil.h"

using namespace Antix;

int main(int argc, char** argv)
{
    setbuf(stdout, NULL);

    Network::RobotClient rclient;
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
    ClientList clients;
    
    parseServerFile(server_fn,grid_servers,robot_clients, clock_server, draw_server); 
    parseConfigFile(config_fn,clients);
    
    rclient.init();
    
    // Connect to all the grid servers
    for(ConListIterator iter = grid_servers.begin();
        iter != grid_servers.end();
        iter++)
    {
        rclient.initGrid( (*iter).first.c_str(), (*iter).second.c_str() );
    }
    
    for( ClientList::iterator iter = clients.begin();
         iter != clients.end();
         iter++)
    {
        std::vector< TeamGridPair > pairs = iter->second;
        for(int i = 0; i < pairs.size(); i++)
        {
            cout << "TeamNum: " << pairs[i].first;
            cout << "\tGridNum: " << pairs[i].second << endl;;
        }
    }


    //Connect to the clock
    rclient.initClock(clock_server.first.c_str(), clock_server.second.c_str());
    rclient.start();

    return 0;
}

