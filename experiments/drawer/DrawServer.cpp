#include "DrawServer.h"

using namespace Network;

DrawServer* DrawServer::m_instance = NULL;

DrawServer::DrawServer()
{
    this->m_posDB = new AntixUtils::Logger();
    if (this->m_posDB == NULL)
    {
        printf("Redis position DB is not initialized");
        return;
    }
    this->m_posDB->setDataOnly(true);
}

DrawServer::~DrawServer() 
{
}

DrawServer* DrawServer::getInstance()
{
    if (!m_instance)
        m_instance = new DrawServer();

    return m_instance;
}

void DrawServer::init(int argc, char** argv)
{
    this->m_windowSize = 600;
    this->m_worldSize = 1.0;

    int teamCount=1, puckCount=0, popCount=0;
    DrawUtils::parseOptions(argc, argv, &teamCount, &puckCount, &popCount, 
            &(this->m_windowSize), &(this->m_worldSize), &(this->m_FOVAngle), &(this->m_FOVRange));
    this->m_teamRadius = 0.0;

    /* Fake generate team data */
    for(int i=0; i<teamCount; i++)
    {
        Game::Team *team = new Game::Team;
        team->posX = i*100;
        team->posY = i*100;
        team->colR = i;
        team->colG = i;
        team->colB = i;
        this->m_teams.push_back(team);
    }
}

void DrawServer::update()
{
    char logKey[16];
    sprintf(logKey, "%s%d", POS_DB_NAME, (this->m_timestep % MAX_POS_KEYS));
    this->m_posDB->setLogKey(logKey);
    cout << "\n*** LogKey=" << logKey << endl;

    this->m_pucks.clear();
    this->m_robots.clear();

    clock_t start = clock();

    vector<AntixUtils::LogItem> items = this->m_posDB->logitems();
    for(vector<AntixUtils::LogItem>::iterator it = items.begin(); it!=items.end(); it++)
    {
        const char* msgBuf = (*it).message.c_str();
        float posX, posY, orientation;
        int team;
        char hasPuck;

        sscanf(msgBuf, POS_PATTERN, &posX, &posY, &orientation, &team, &hasPuck);
        if (team == 0)
        {
            Game::Puck *puck = new Game::Puck;
            puck->posX = posX;
            puck->posY = posY;
            this->m_pucks.push_back(puck);
        }
        else
        {
            Game::Robot *robot = new Game::Robot;
            robot->posX = posX;
            robot->posY = posY;
            robot->orientation = orientation;
            robot->team = this->m_teams[team-1];
            robot->hasPuck = hasPuck == 'T';
            this->m_robots.push_back(robot);
        }
        /*
        printf("%s: %f %f %f %d %c\n",
                (team==0?"Puck":"Robot"), posX, posY, orientation, team, hasPuck);
        //cout << "\tString=" << (*it).message << endl;
        //cout << "\tChar* =" << msgBuf << endl;
         */
    }

    double elapsed = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
    cout << this->m_timestep << ": " << elapsed << "ms (Robots=" << 
            this->m_robots.size() << ", Pucks=" << this->m_pucks.size() << ")" << endl;
}