#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "RobotParser.h"
#include "RobotClient.h"
#include "Config.h"


int main(int argc, char** argv)
{
    setbuf(stdout, NULL);

    if (argc <2)
    {
        printf("Usage: ./client.bin <init_file>\n");
        return -1;
    }

    Network::RobotClient * l_rclient = new Network::RobotClient;
    l_rclient->init();
	
	RobotParser l_parser;
	
	int l_res = 0;
	DEBUGPRINT("About to readfile\n");
	if ((l_res = l_parser.readFile(argv[1], (void *)l_rclient )) == ENOENT) 
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

    l_rclient->start();
/*
char* server_info_file = NULL;
char* system_config_file = NULL;
int client_num = -1;
=======
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
    rclient.start();*/

    return 0;
}

