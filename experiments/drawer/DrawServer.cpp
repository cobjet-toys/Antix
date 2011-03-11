#include "DrawServer.h"
#include <string>

using namespace Network;

DrawServer* DrawServer::m_instance = NULL;

DrawServer::DrawServer()
{
    this->m_redisCli = new AntixUtils::Logger();
    if (this->m_redisCli == NULL)
    {
        printf("Redis position DB is not initialized");
        return;
    }
    
    this->m_redisCli->setDataOnly(true);
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
    this->m_homeRadius = 1.0;
    this->m_FOVEnabled = false;

    if (argc > 1)
    {
        //<window_size> <world_size> <home_radius> <enable_FOV> [ <FOV_angle> <FOV_range> ]
        this->m_windowSize = atoi(argv[1]);
        this->m_worldSize = strtof(argv[2], NULL);
        this->m_homeRadius = strtof(argv[3], NULL);
        this->m_FOVEnabled = atoi(argv[4]);
        if (this->m_FOVEnabled)
        {
            this->m_FOVAngle = strtof(argv[5], NULL);
            this->m_FOVRange = strtof(argv[6], NULL);
        }
    }

    initTeams();
}

void DrawServer::initTeams()
{
    AntixUtils::LogItem * teamData;
    this->m_redisCli->setLogKey(TEAM_DB_NAME);

    while ((teamData = this->m_redisCli->blpop()) != NULL)
    {
        int teamID, colR, colG, colB;
        float homeX, homeY;

        sscanf(teamData->message.c_str(), TEAM_PATTERN, &teamID, &homeX, &homeY, &colR, &colG, &colB);
        Game::Team * team = new Game::Team;
        team->posX = homeX;
        team->posY = homeY;
        team->colR = colR;
        team->colG = colG;
        team->colB = colB;
        this->m_teams[teamID] = team;
    }

    printf("Teams=%d\n", this->m_teams.size());
}

void DrawServer::update()
{
    char logKey[16];
    sprintf(logKey, "%s%d", POS_DB_NAME, (this->m_framestep % MAX_POS_KEYS));
    this->m_redisCli->setLogKey(logKey);
    cout << "\n*** LogKey=" << logKey << endl;

    this->m_pucks.clear();
    this->m_robots.clear();

    clock_t start = 0;
    vector<AntixUtils::LogItem> items;

    do
    {
        if (start != 0)
        {
            //TO-DO: is this thread-safe? how will it interact with openGL?
            sleep(2);
        }
        start = clock();
        items = this->m_redisCli->logitems();
    } while(items.size() == 0);

    for(vector<AntixUtils::LogItem>::iterator it = items.begin(); it!=items.end(); it++)
    {
        const char* msgBuf = (*it).message.c_str();
        float posX, posY, orientation;
        uint32_t id;
        char hasPuck;

        sscanf(msgBuf, POS_PATTERN, &id, &posX, &posY, &orientation, &hasPuck);
        if (id/TEAM_ID_SHIFT == 0)
        {
            Game::Puck *puck = new Game::Puck;
            puck->posX = posX;
            puck->posY = posY;
            this->m_pucks[id] = puck;
        }
        else
        {
            Game::Robot *robot = new Game::Robot;
            robot->posX = posX;
            robot->posY = posY;
            robot->orientation = orientation;
            robot->team = this->m_teams[id/TEAM_ID_SHIFT];
            robot->hasPuck = hasPuck == 'T';
            this->m_robots[id] = robot;
        }
        
        printf("%s: %d %f %f %f %c\n",
                (id==0?"Puck":"Robot"), id, posX, posY, orientation, hasPuck);
        //cout << "\tString=" << (*it).message << endl;
        //cout << "\tChar* =" << msgBuf << endl;
         /**/
    }

    this->m_redisCli->clear();

    double elapsed = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
    printf("%d: %fms (Robots=%d, Pucks=%d)\n",
            this->m_framestep, elapsed, this->m_robots.size(), this->m_pucks.size());
    this->m_framestep++;
}