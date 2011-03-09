#include "Network/Client.h"
#include "Network/Packer.h"
#include "Game/Game.h"

int m_ClientState = 0;

void sendData(int &sockfd, unsigned char* buffer, int &msgSize)
{
    //Bytes sent.
    int l_Sent = 0;

    while (l_Sent < msgSize)
        l_Sent += send(sockfd, buffer+l_Sent, msgSize-l_Sent, 0);
}


void recvData(int &sockfd, unsigned char* buffer, int &msgSize)
{
    //Bytes received.
    int l_Recv = 0;

    while (l_Recv < msgSize)
        l_Recv += recv(sockfd, buffer+l_Recv, msgSize-l_Recv, 0);
}

void recvInitConfig(int &sockfd)
{
     //44byte message
    int l_PuckCount, l_WindowSize, l_RobPop, l_RobOffset, l_TeamCount;
    float l_WorldSize, l_RobFOV, l_RobRadius, l_RobSensorRange, l_RobPickupRange, l_HomeRadius;

    //If you add config options, change this too.
    int l_MessageSize = 44;    
    unsigned char l_Buffer[l_MessageSize];
    
    //Receive our message.
    recvData(sockfd, l_Buffer, l_MessageSize);

  
    //Pack our message.
    unpack(l_Buffer, "lllllffffff", &l_PuckCount, &l_WindowSize, &l_RobPop, &l_RobOffset, &l_TeamCount,
            &l_WorldSize, &l_RobFOV, &l_RobRadius, &l_RobSensorRange, &l_RobPickupRange, &l_HomeRadius);

    printf("Received configuration options:\n"); 
    printf("%d %d %d %d %d %f %f %f %f %f %f\n", l_PuckCount, l_WindowSize, l_RobPop, l_RobOffset, l_TeamCount,
            l_WorldSize, l_RobFOV, l_RobRadius, l_RobSensorRange, l_RobPickupRange, l_HomeRadius);

    //Configure the game.
    using namespace Game;
    Robotix* l_Game = Robotix::getInstance();
    l_Game->setWindowSize(l_WindowSize);
    l_Game->setWorldSize(l_WorldSize);
    l_Game->setPuckCount(l_PuckCount);
    l_Game->setTeamCount(l_TeamCount);
    l_Game->setRobOffset(l_RobOffset);
    l_Game->setRobSensorRange(l_RobSensorRange);
    l_Game->setRobFOV(l_RobFOV);
    l_Game->setRobPickupRange(l_RobPickupRange);
    l_Game->setRobRadius(l_RobRadius);
    l_Game->setHomeRadius(l_HomeRadius);
    l_Game->setRobPop(l_RobPop);

    //Initialize the game.
    l_Game->init();

    float l_Xpos, l_Ypos, l_Orient;
    l_MessageSize = 12;
    for(std::list<Team*>::iterator it = Robotix::getInstance()->getFirstTeam(); it != Robotix::getInstance()->getLastTeam(); it++)
    {
        Position* l_Pos = (*it)->getHome()->getPosition();
        recvData(sockfd, l_Buffer, l_MessageSize);
        unpack(l_Buffer, "fff", &l_Xpos, &l_Ypos, &l_Orient);

        l_Pos->setX(l_Xpos);
        l_Pos->setY(l_Ypos);
        l_Pos->setOrient(l_Orient);
    } 
}

void sendAck(int &sockfd)
{
    int l_Response = 1337;

    int l_MessageSize = 4;
    unsigned char l_Buffer[l_MessageSize];

    pack(l_Buffer, "l", l_Response);
    sendData(sockfd, l_Buffer, l_MessageSize);
}

void sendGameInfo(int &sockfd)
{
     using namespace Game;
    int l_MessageSize = 16;
    unsigned char l_Buffer[l_MessageSize];

    
    //Send the rob positions only for the client's robots.
    int l_TeamPop = Robotix::getInstance()->getRobPop()+Robotix::getInstance()->getRobOffset();
    for(int i = Robotix::getInstance()->getRobOffset(); i < l_TeamPop; i++ )
    {
       Position* l_Pos = Robotix::getInstance()->getRobotAtIndex(i)->getPosition();
       pack(l_Buffer, "lfff", i, l_Pos->getX(), l_Pos->getY(), l_Pos->getOrient());
       sendData(sockfd, l_Buffer, l_MessageSize); 
    }
    
    //Puck messages are smaller because they lack an id.
    l_MessageSize = 12;
    //Send the puck positions.
    for(std::list<Puck*>::iterator it = Robotix::getInstance()->getFirstPuck(); it != Robotix::getInstance()->getLastPuck(); it++)
    {
        Math::Position* l_Pos = (*it)->getPosition();
        
        pack(l_Buffer, "fff", l_Pos->getX(), l_Pos->getY(), l_Pos->getOrient() );
        sendData(sockfd, l_Buffer, l_MessageSize);
    }
}

void recvGameInfo(int &sockfd)
{
    using namespace Game;
    int l_MessageSize = 12;
    unsigned char l_Buffer[l_MessageSize];

    //Recv data unpacked to here;.
    float l_Xpos, l_Ypos, l_Orient;

    //Recv the robot positions.
    for(std::vector<Robot*>::iterator it = Robotix::getInstance()->getFirstRobot(); it != Robotix::getInstance()->getLastRobot(); it++)
    {
        Math::Position* l_Pos = (*it)->getPosition();
        recvData(sockfd, l_Buffer, l_MessageSize);
        
        unpack(l_Buffer, "fff", &l_Xpos, &l_Ypos, &l_Orient);
        l_Pos->setX(l_Xpos);
        l_Pos->setY(l_Ypos);
        l_Pos->setOrient(l_Orient); 
    }
    
    //Recv the puck positions.
    for(std::list<Puck*>::iterator it = Robotix::getInstance()->getFirstPuck(); it != Robotix::getInstance()->getLastPuck(); it++)
    {
        Math::Position* l_Pos = (*it)->getPosition();
        recvData(sockfd, l_Buffer, l_MessageSize);
        
        unpack(l_Buffer, "fff", &l_Xpos, &l_Ypos, &l_Orient);
        l_Pos->setX(l_Xpos);
        l_Pos->setY(l_Ypos);
        l_Pos->setOrient(l_Orient); 
     }
}

void* InitClient(void* server)
{
    Server* l_Server = (Server*)server;
    struct addrinfo hints, *servinfo, *p;
    int rv, sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(l_Server->destination, l_Server->port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(1);
    }
    printf("client: connected\n");

    freeaddrinfo(servinfo); // all done with this structure

    while(1)
    {
        switch(m_ClientState)
        {
            //Initializing.
            case(0):
                recvInitConfig(sockfd);
                sendAck(sockfd);
                m_ClientState = 1;
                break;
            //Ready to receive data and process.
            case(1):
                recvGameInfo(sockfd);
                m_ClientState = 2;
                break;
            //Do one update, then send data.
            case(2):
                Game::Robotix::getInstance()->update(); 
                Game::Robotix::getInstance()->update(); 
                sendGameInfo(sockfd);
                m_ClientState = 1;
                break;
        }
    }

}

