#include "DrawServer.h"
#include "Common/Home.h"
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
    
    this->m_windowSize = 600;
    this->m_worldSize = 1.0;
    this->m_FOVEnabled = false;
    this->m_FOVAngle = 0.0;
    this->m_FOVRange = 0.0;
    this->m_homeRadius = 20.0;
    this->m_framestep = 0;
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
        Math::Position *homePos = new Math::Position(homeX, homeY, 0.0);
        Game::Home * home = new Game::Home(homePos);
        
        //figure something out for the colour
        
        this->m_teams[teamID] = new Game::Team();
        this->m_teams[teamID]->m_Home = home;
    }
    
    if (this->m_teams.size() == 0)
    {
    	Math::Position *homePos = new Math::Position(55.0, 150.0, 0.0);
        Game::Home * home = new Game::Home(homePos);        
        this->m_teams[1] = new Game::Team();
        this->m_teams[1]->m_Home = home;
    }

    printf("Teams=%d\n", this->m_teams.size());
}

void DrawServer::update()
{
    char logKey[16];
    sprintf(logKey, "%s%d", POS_DB_NAME, (this->m_framestep % MAX_POS_KEYS));
    this->m_redisCli->setLogKey(logKey);
    //cout << "\n*** LogKey=" << logKey << endl;

    clock_t start = clock();
    vector<AntixUtils::LogItem> items;

    items = this->m_redisCli->logitems();
    
    // Print time in (ms) that it took to fetch data 
    //double elapsed = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
    //printf("Fetch   %d: %fms (Robots=%d, Pucks=%d)\n", this->m_framestep, elapsed, this->m_robots.size(), this->m_pucks.size());
    
    start = clock();
    
    if (items.size() == 0)
    {
    	cout << "No items in " << logKey << endl;
    	return;
    	
    }

    for(vector<AntixUtils::LogItem>::iterator it = items.begin(); it!=items.end(); it++)
    {
        const char* msgBuf = (*it).message.c_str();
        float posX, posY, orientation;
        uint32_t id;
        char hasPuck;

        sscanf(msgBuf, POS_PATTERN, &id, &posX, &posY, &orientation, &hasPuck);
        
        Math::Position *pos = new Math::Position(posX, posY, orientation);
        if (!pos)
        { 
        	printf("invalid pos");
        	continue;
        }
        
        if(id/TEAM_ID_SHIFT == 0)
        {
            if (!this->m_pucks[id])
            {
            	  this->m_pucks[id] = new Game::Puck(pos);
            }
          	else
          	{
          		  this->m_pucks[id]->getPosition()->setX(posX);
          		  this->m_pucks[id]->getPosition()->setY(posY);        		
          	}
        }
        else
        {
          	if (!this->m_robots[id])
          	{
            		if (!this->m_teams[1])//id/TEAM_ID_SHIFT])    
            		{
            			  printf("no home for %d", 1);//id/TEAM_ID_SHIFT);
            			  continue;
            		}         			
      			    this->m_robots[id] = new Game::Robot(
          		  pos, this->m_teams[1]->getHome());//id/TEAM_ID_SHIFT]->getHome());
          	}
              	
          	else
          	{
            		this->m_robots[id]->getPosition()->setX(posX);
            		this->m_robots[id]->getPosition()->setY(posY);        		
          	}
            
            //this->m_robots[id]->m_PuckHeld = hasPuck == 'T';
        }
        
        //printf("%s: %d %f %f %f %c\n",
        //        (id==0?"Puck":"Robot"), id, posX, posY, orientation, hasPuck);
        //cout << "\tString=" << (*it).message << endl;
        //cout << "\tChar* =" << msgBuf << endl;
         /**/
    }

    //this->m_redisCli->clear();

    // Print time in (ms) that it took to process data
    //double elapsed2 = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
    //printf("Process %d: %fms (Robots=%d, Pucks=%d)\n",
            //this->m_framestep, elapsed2, this->m_robots.size(), this->m_pucks.size());
    
    //check for overflow
    this->m_framestep++;
}
