#include "GridServer.h"
#include "FileUtil.h"

using namespace Antix;

int main(int argc, char ** argv)
{
	Network::GridServer grid;
	if (argc < 3) 
	{
		perror("Usage: ./grid.bin <server_info_file> <grid_port>");
		return -1;
	}
    const char* server_fn = argv[1];
    const char* port_num = argv[2];

    ConnectionList grid_servers;
    ConnectionList robot_clients;
    ConnectionPair clock_server;
    ConnectionPair draw_server;
    
    parseServerFile(server_fn,grid_servers,robot_clients, clock_server, draw_server); 
	
    if (grid.init(port_num) < 0) 
	{
		return -1;
	}
	
	grid.start();
	
	return 0;
}
