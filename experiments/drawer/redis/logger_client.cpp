#include "redislist.h"
#include <iostream>
#include <pthread.h>
#include <time.h>

using namespace AntixUtils;

#define FRAME_FREQUENCY 33

void init(string db_host, int homes, int pucks, int homePop)
{
    this->posDB = new AntixUtils::Logger(db_host, DEFAULT_PORT, DEFAULT_DBINDEX);
    if (!this->posDB)
    {
        printf("Redis position DB is not initialized");
        return;
    }

    this->posDB->setDataOnly(true);
    this->m_totalHomes = homes;
    this->m_totalPucks = pucks;
    this->m_totalRobots = homePop;

    /* Push fake team data */
    this->posDB->setLogKey(TEAM_DB_NAME);
    for(int i=0; i<homes; i++)
    {
        int team_id = i+1;
        float posX = 55.0;
        float posY = 150.0;
        int colR = 255;
        int colG = 255;
        int colB = 255;
        char buf[64];
        bzero(buf, 64);
        sprintf(buf, TEAM_PATTERN, team_id, posX, posY, colR, colG, colB);
        cout << buf << endl;
        this->posDB->append(string(buf));
    }
}


void * push_function(void* args)
{

        init(dbhost, 1, 5, 10);
        while (true)
        {
                usleep(FRAME_FREQUENCY);

                try
                {
                         update();
                }
                catch (std::exception & e)
                {
                        cerr << "got exception: " << e.what() << endl << "FAIL" << endl;
                        return;
                }
        }
}

int main()
{
        pthread_t thread1;
        int iret1;

        iret1 = pthread_create( &thread1, NULL, push_function, NULL);
        if (iret1 != 0 || pthread_join(&thread1, NULL ) != 0)
        {
                cout << "pthread failed" << endl;
                return -1;
        }


        cout << endl << "testing completed successfully" << endl;
        return 0;
}
