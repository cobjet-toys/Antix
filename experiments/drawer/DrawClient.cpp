#include "DrawClient.h"

using namespace Network;

DrawClient::DrawClient()
{
    this->posDB = new AntixUtils::Logger();
    this->m_totalHomes = 1;
    this->m_totalPucks = 1000;
    this->m_totalRobots = 10;
}

void DrawClient::update()
{
    if (this->posDB == NULL)
    {
        printf("Redis position DB is not initialized");
        return;
    }

    cout << "\n--------------DrawClient----------------\n" << endl;

    this->posDB->clear();
    for(int i=0; i<(this->m_totalRobots); i++)
    {
        char buf[256];
        int home, hasPuck;
        float posX, posY, orientation;

        bzero(buf, 256);
        home=i%3;
        hasPuck=i%2;
        posX=(float)i;
        posY=posX;
        orientation = 1.0;

        sprintf(buf, "%f %f %f %d %d", posX, posY, orientation, home, hasPuck);
        //sprintf(buf, "%f %f", posX, posY);
        cout << buf << endl;
        this->posDB->append(string(buf));
    }

    for(int i=0; i<(this->m_totalPucks); i++)
    {

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