#include "DrawServer.h"

using namespace Network;

DrawServer::DrawServer()
{
    this->posDB = new AntixUtils::Logger();
    this->m_totalHomes = 1;
    this->m_totalPucks = 1000;
    this->m_totalRobots = 30000;
}

DrawServer::~DrawServer() {
}

void DrawServer::draw()
{
    if (this->posDB == NULL)
    {
        printf("Redis position DB is not initialized");
        return;
    }

    cout << "\n--------------DrawServer----------------\n" << endl;

    vector<AntixUtils::LogItem> items = this->posDB->logitems();
    for(unsigned int i=0; i<items.size(); i++)
    {
        cout << items[i].message << endl;
    }
}