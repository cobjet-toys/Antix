#include "GridServer.h"
#include "GridParser.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define FRAME_FREQUENCY 500000

void * drawer_function(void* gridPtr)
{
    Network::GridServer * grid = (Network::GridServer *)gridPtr;

    for(uint32_t frame = 0; true; frame++)
    {
        usleep(FRAME_FREQUENCY);

        try
        {
            printf("Updated Drawer!\n");    		
            grid->updateDrawer(frame);
        }
        catch (std::exception & e)
        {
            printf("Failed To Updated Drawer!\n");
            perror(e.what());
            return NULL;
        }
    }

    printf("Finished Execution!!\n");   
	return NULL;
}

int main(int argc, char ** argv)
{
	Network::GridServer *l_grid = new Network::GridServer();
	if (argc < 3) 
	{
		printf("Usage: ./grid.bin <port> <init_file>\n");
		return -1;
	}
	
	GridParser l_parser;
	
	int l_res = 0;
	
	if ((l_res = l_parser.readFile(argv[2], (void *)l_grid )) == ENOENT) 
	{
		printf("Error with parsing file: %s\n", argv[2]);
		return -1;
	}
	if (l_res < 0)
	{
		printf("Failed to parse file\n");
		return -1;
	}
	
	if (l_grid->init(argv[1]) < 0) 
	{
		return -1;
	}
	
	
	//Spawn Drawer thread
	
	pthread_t thread1;
    int iret1;
    
    iret1 = pthread_create(&thread1, NULL, drawer_function, (void *)&l_grid);
    if(iret1 != 0 || pthread_join(thread1, NULL) != 0)
    {
        perror("pthread failed");
        return -1;
    }
	
	l_grid->start();
	
	return 0;
}
