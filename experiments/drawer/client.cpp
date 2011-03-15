#include "DrawClient.h"
#include <pthread.h>

#define FRAME_FREQUENCY 30

void * drawer_function(void* dCliPtr)
{
    Network::DrawClient * dCli = (Network::DrawClient *)dCliPtr;

    for(uint32_t frame=0; true ; frame++)
    {
        usleep(FRAME_FREQUENCY);

        try
        {
    		dCli->update(frame);
        }
        catch (std::exception & e)
        {
            cerr << "got exception: " << e.what() << endl << "FAIL" << endl;
            return NULL;
        }
    }
}


int main(int argc, char** argv)
{
    Network::DrawClient * dCli = new Network::DrawClient();
    
    //fake extract drawer address from server.info
    string dbhost = argc == 1? "localhost" : argv[1];
    int homes = argc == 1? 1 : atoi(argv[2]);
    int pucks = argc == 1? 5 : atoi(argv[3]);
    int homePop = argc == 1? 10 : atoi(argv[4]);
    
    pthread_t thread1;
    int iret1;

    dCli->init(dbhost, homes, pucks, homePop);

    iret1 = pthread_create( &thread1, NULL, drawer_function, (void *)dCli);
    if (iret1 != 0 || pthread_join(thread1, NULL ) != 0)
    {
            cout << "pthread failed" << endl;
            return -1;
    }
    

    return (EXIT_SUCCESS);
}

