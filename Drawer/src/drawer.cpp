/* 
 * File:   drawer.cpp
 * Author: cla18
 *
 * Created on March 6, 2011, 11:22 PM
 */

#include <stdlib.h>
#include "DrawServer.h"
#include "DrawerParser.h"
#include "Gui.h"

int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		perror("Please specify a config file for the Drawer");
		return (EXIT_FAILURE);
	}
    
    Network::DrawServer * drawer = Network::DrawServer::getInstance();
	drawer->init();
	
	//parse options
    int c;    
	while( ( c = getopt( argc, argv, "f:")) != -1 )
	{
		switch( c )
		{				
			case 'f':
			{
				DrawerParser l_parser;	
				int l_res = 0;
				if ((l_res = l_parser.readFile(optarg, (void *)drawer )) == ENOENT) 
				{
					printf("Error with parsing file: %s\n", argv[2]);
					return -1;
				}
				if (l_res < 0)
				{
					printf("Failed to parse file\n");
					return -1;
				}
				break;
			}
				
			default:
			{
				//DEBUGPRINT("Option parse error. Usage: ./drawer.bin -f <config_file>\n" );
				//return (EXIT_FAILURE);
				break;
			}
		}
	}	
	

    initGraphics(argc, argv);
    
    return (EXIT_SUCCESS);
}

