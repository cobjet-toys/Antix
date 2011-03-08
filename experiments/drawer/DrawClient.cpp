#include "DrawClient.h"

using namespace Network;

DrawClient::DrawClient(int homes, int pucks, int homePop)
{
    this->posDB = new AntixUtils::Logger();
    if (this->posDB == NULL)
    {
        printf("Redis position DB is not initialized");
        return;
    }
    this->posDB->setDataOnly(true);
    
    this->m_totalHomes = homes;
    this->m_totalPucks = pucks;
    this->m_totalRobots = homePop;
}

DrawClient::~DrawClient() 
{
}

void DrawClient::update()
{
    cout << "\n--------------DrawClient----------------\n" << endl;
    cout << "*** Objects=" << (this->m_totalRobots + this->m_totalPucks) << endl;
    cout << "*** Steps=" << ULONG_MAX << endl << endl;

    for(uint32_t j=0; j<3; j++)
    {
        char logKey[16];
        sprintf(logKey, "%s%d", POS_DB_NAME, j%MAX_POS_KEYS);
        this->posDB->setLogKey(logKey);
        this->posDB->clear();
        cout << "\n*** LogKey=" << logKey << endl;

        clock_t start = clock();
        for(int i=0; i<(this->m_totalRobots); i++)
        {
            char buf[64];
            int home;
            char hasPuck;
            float posX, posY, orientation;

            bzero(buf, 64);
            home=i%3+1;
            hasPuck=i%2==0?'T':'F';
            posX=(float)i;
            posY=posX;
            orientation = 1.0;

            sprintf(buf, POS_PATTERN, posX, posY, orientation, home, hasPuck);
            //cout << buf << " (" << strlen(buf) << ")" << endl;
            this->posDB->append(string(buf));
        }

        int x = this->m_totalRobots;        /* used to fake <x,y> of puck */
        for(int i=0; i<(this->m_totalPucks); i++)
        {
            char buf[256];
            float posX, posY;

            bzero(buf, 256);
            posX=(float)i+x;
            posY=posX;

            sprintf(buf, POS_PATTERN, posX, posY, 0.0, 0, 'F');
            //cout << buf << endl;
            this->posDB->append(string(buf));
        }

        double elapsed = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
        cout << j << ": " << elapsed << "ms" << endl;
    }
    

    /*
    std::vector< pair<string, string> > data;

    for (int i=0; i < (this->m_totalRobots); i++)
    {
        char key[16], posX[16], posY[16], orientation[16], home[16], hasPuck[16];
        sprintf(key, "%d", i);
        sprintf(posX, "%d", i);
        sprintf(posY, "%d", i);
        sprintf(orientation, "%d", i);
        sprintf(home, "%d", i%3);
        sprintf(hasPuck, "%d", i%2);

        data.push_back(make_pair(FIELD_POSX, posX));
        data.push_back(make_pair(FIELD_POSY, posY));
        data.push_back(make_pair(FIELD_ORIENTATION, orientation));
        data.push_back(make_pair(FIELD_HOME, home));
        data.push_back(make_pair(FIELD_HASPUCK, hasPuck));

        this->posDB->hmset(key,data);
        data.clear();
    }

    for (int i=0; i < this->m_totalPucks; i++)
    {
        char key[16], posX[16], posY[16];
        sprintf(key, "%d", i);
        sprintf(posX, "%d", i);
        sprintf(posY, "%d", i);

        data.push_back(make_pair(FIELD_POSX, posX));
        data.push_back(make_pair(FIELD_POSY, posY));

        this->posDB->hmset(key,data);
        data.clear();
    }
     * */

}
