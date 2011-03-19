#include "GridServer.h"
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
	Network::GridServer grid;
	if(argc < 2) 
	{
		perror("Please specify a port for the server");
		return -1;
	}
	if(grid.init(argv[1]) < 0) 
	{
		return -1;
	}

	pthread_t thread1;
    int iret1;

    //grid.initDrawer();

    iret1 = pthread_create(&thread1, NULL, drawer_function, (void *)&grid);
    if(iret1 != 0 || pthread_join(thread1, NULL) != 0)
    {
        perror("pthread failed");
        return -1;
    }
	
	grid.start();
	
	return 0;
}
