#include "DrawClient.h"

using namespace Network;

DrawClient::DrawClient()
{
}

DrawClient::~DrawClient() 
{
}

void DrawClient::init(string db_host, int homes, int pucks, int homePop)
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
        int team_id = i;
        float posX = i*10;
        float posY = posX;
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

void DrawClient::update()
{
    cout << "\n--------------DrawClient----------------\n" << endl;
    cout << "*** Objects=" << (this->m_totalRobots + this->m_totalPucks) << endl;
    cout << "*** Steps=" << 3 << endl << endl;

    for(uint32_t j=0; j<3; j++)
    {
        char logKey[8];
        sprintf(logKey, "%s%d", POS_DB_NAME, j%MAX_POS_KEYS);
        this->posDB->setLogKey(logKey);
        cout << "\n*** LogKey=" << logKey << endl;

        clock_t start = clock();
        for(int i=0; i<(this->m_totalRobots); i++)
        {
            char buf[64];
            int id;
            char hasPuck;
            float posX, posY, orientation;

            bzero(buf, 64);
            id=i+1024;
            posX=(float)i;
            posY=posX;
            orientation = 1.0;
            hasPuck=i%2==0?'T':'F';

            sprintf(buf, POS_PATTERN, id, posX, posY, orientation, hasPuck);
            cout << buf << endl;
            this->posDB->append(string(buf));
        }

        int x = this->m_totalRobots;        /* used to fake <x,y> of puck */
        for(int i=0; i<(this->m_totalPucks); i++)
        {
            char buf[64];
            float posX, posY;

            bzero(buf, 64);
            posX=(float)i+x;
            posY=posX;

            sprintf(buf, POS_PATTERN, 0, posX, posY, 0.0, 'F');
            cout << buf << endl;
            this->posDB->append(string(buf));
        }

        double elapsed = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
        cout << j << ": " << elapsed << "ms" << endl;
    }

}
