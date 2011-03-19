#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "RobotParser.h"
#include "RobotClient.h"
#include "Config.h"

const char usage[] = "Required arguments for the RobotClient:\n"
    "  -? : Prints this helpful message.\n"
    "  -i <server_info_file> : Specifies the file to read for server info\n"
    "  -c <system_config_file> : Specifies the system config file to read \n"
    "  -n <client_num> : Specifies the client number\n";

int main(int argc, char** argv)
{

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

  // parse arguments to configure Robot static members
	int c;
	while( ( c = getopt( argc, argv, "?d:i:g:s:")) != -1 )
		switch( c )
			{
			case 'i':
			  server_info_file = optarg ;
			  printf( "server info filename set: %s\n", server_info_file );
			  break;
			case 'c':
			  system_config_file = optarg;
			  printf( "system config file set: %s\n", system_config_file );
			  break;
			case 'n':
			  client_num = atoi(optarg);
			  printf( "starting grid set: %d\n", client_num );
			  break;
			case '?':
			  puts( usage );
			  exit(0); // ok
			  break;
			default:
				fprintf( stderr, "[Team] Option parse error.\n" );
				puts( usage );
				exit(-1); // error
			}

	if (server_info_file == NULL | system_config_file == NULL | client_num == -1)
	{
		fprintf( stderr, "You have not entered all required args\n" );
		puts( usage );
		exit(-1); // error
	}
*/
	//printf(usage);
    return 0;
}
