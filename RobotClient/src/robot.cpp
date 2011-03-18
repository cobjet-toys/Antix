#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "RobotClient.h"

const char usage[] = "Required arguments for the RobotClient:\n"
    "  -? : Prints this helpful message.\n"
    "  -i <server_info_file> : Specifies the file to read for server info\n"
    "  -c <system_config_file> : Specifies the system config file to read \n"
    "  -n <client_num> : Specifies the client number\n";

int main(int argc, char** argv)
{

    if (argc <3)
    {
        printf("Specify IP and Port\n");
        return -1;
    }

    Network::RobotClient rclient;

    rclient.init();

    //TODO Get grid servers
    rclient.initGrid("142.58.35.87", "22222");
    rclient.initGrid("142.58.35.218", "33333");
    rclient.initGrid("142.58.35.74", "44444");
    rclient.initGrid("142.58.35.171", "55555");
    rclient.initGrid("142.58.35.101", "66666");

    //Connect to the cloc
    rclient.initClock("142.58.35.211", "77777");

    rclient.start();
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
