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
    this->m_totalRobots = 1000;//homePop;

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

void DrawClient::update(uint32_t framestep)
{
    cout << "\n--------------DrawClient----------------\n" << endl;
    cout << "*** Objects=" << (this->m_totalRobots + this->m_totalPucks) << endl;

    // Creates an array of random values, to draw the pucks -- TEMPORARY TESTING -- //
    srand(time(NULL));
    float randPuckVals[this->m_totalPucks * 2];
    for(int i = 0; i < this->m_totalPucks * 2; i++){randPuckVals[i] = (float)(rand()%600);}
    // Creates an array of random values for robot positions -- TEMPORARY TESTING -- //
    //float robotOrientation[this->m_totalRobots];
    //for(int i = 0; i < this->m_totalRobots; i++){robotPosition[i] = (float)(rand()%100)/100;}
    float robotPosition[this->m_totalRobots][2];
    for(int i = 0; i < this->m_totalRobots; i++){for(int j = 0; j < 2; j++){robotPosition[i][j] = (float)(rand()%600);}}

    char logKey[8];
    sprintf(logKey, "%s%d", POS_DB_NAME, framestep%MAX_POS_KEYS);
    this->posDB->setLogKey(logKey);
    //cout << "\n*** LogKey=" << logKey << endl;

    clock_t start = clock();
    for(int i=0; i < this->m_totalRobots; i++)
    {
        char buf[64];
        int id;
        char hasPuck;
        float posX, posY, orientation;

        bzero(buf, 64);
        id = i + 1024;

        // Computes robots altered position (Random)
        robotPosition[i][0] += float((rand()%200)-100)/50;
        robotPosition[i][1] += float((rand()%200)-100)/50;

        posX = robotPosition[i][0];//(float)i*10+j*5;
        posY = robotPosition[i][1];//i*15+j*3;
        //posX = (float)i*10+j*5;
        //posY = i*15+j*3;
        orientation = 1.0;
        hasPuck = i%2 == 0 ? 'T':'F';

        sprintf(buf, POS_PATTERN, id, posX, posY, orientation, hasPuck);
        //cout << buf << endl;
        this->posDB->append(string(buf));
    }

    for(int i=0; i < this->m_totalPucks; i++)
    {
        char buf[64];
        float posX, posY;

        bzero(buf, 64);
        posX = randPuckVals[(2*i)];
        posY = randPuckVals[(2*i) + 1];

        sprintf(buf, POS_PATTERN, i, posX, posY, 0.0, 'F');
        //cout << buf << endl;
        this->posDB->append(string(buf));
    }

    double elapsed = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
    cout << framestep << ": " << elapsed << "ms" << endl;

}
