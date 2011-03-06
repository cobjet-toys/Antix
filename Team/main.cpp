#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>


const char usage[] = "Required arguments for team:\n"
    "  -? : Prints this helpful message.\n"
    "  -i <teamid> : Specifies the id of the team \n"
    "  -g <gridmappings> : Specifies the grids in the game, ex: g1=coconut:port,g2=cherry:port\n"
    "  -s <startinggrid> : Specifies the grid the team starts on \n";

int main(int argc, char** argv)
{

unsigned int team_id =  NULL;
char* grid_args = NULL;
char* starting_grid = NULL;

  // parse arguments to configure Robot static members
	int c;
	while( ( c = getopt( argc, argv, "?d:i:g:s:")) != -1 )
		switch( c )
			{
			case 'i':
			  team_id = atoi( optarg );
			  printf( "teamid set: %d\n", team_id );
			  break;
			case 'g':
			  grid_args = optarg;
			  printf( "gridargs set: %s\n", grid_args );
			  break;
			case 's':
			  starting_grid = optarg;
			  printf( "starting grid set: %s\n", starting_grid );
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

	if (team_id == NULL | grid_args == NULL | starting_grid == NULL)
	{
		fprintf( stderr, "You have not entered all required args\n" );
		puts( usage );
		exit(-1); // error
	}

	//printf(usage);
    return 0;
}
