#include "Network/DummyServer.h"
#include "Network/Packer.h"
#include "Game/Game.h"
#include "Math/Position.h"

#include <map>
#include <pthread.h>

std::map<int, int> m_Clients;

void* InitNetwork(void* port)
{
    const char* l_Port = (char*)port;

    int sockfd = 0, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int yes=1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, l_Port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    
    std::list<pthread_t> threads;

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        printf("server: got connection.\n");
        m_Clients[new_fd] = 0; 
        pthread_t thread;
        threads.push_back(thread);
        pthread_create(&threads.back(), NULL, ClientLoop, (void*)new_fd);
    }

    return 0;
}

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
        l_Recv += recv(sockfd, buffer, msgSize, 0);
}

void sendInitConfig(int &sockfd)
{
     //44byte message
    using namespace Game;
    int l_PuckCount = Robotix::getInstance()->getPuckCount();
    int l_WindowSize = Robotix::getInstance()->getWindowSize();
    int l_RobPop = Robotix::getInstance()->getRobPop();
    int l_RobOffset = (m_Clients.size()-1)*l_RobPop;
    int l_TeamCount = Robotix::getInstance()->getTeamCount();
    float l_WorldSize = Robotix::getInstance()->getWorldSize();
    float l_RobFOV = Robotix::getInstance()->getRobFOV();
    float l_RobRadius = Robotix::getInstance()->getRobRadius();
    float l_RobSensorRange = Robotix::getInstance()->getRobSensorRange();
    float l_RobPickupRange = Robotix::getInstance()->getRobPickupRange();
    float l_HomeRadius = Robotix::getInstance()->getHomeRadius();

    //If you add config options, change this too.
    int l_MessageSize = 44;    
    unsigned char l_Buffer[l_MessageSize];

    //Pack our message.
    pack(l_Buffer, "lllllffffff", l_PuckCount, l_WindowSize, l_RobPop, l_RobOffset, l_TeamCount,
            l_WorldSize, l_RobFOV, l_RobRadius, l_RobSensorRange, l_RobPickupRange, l_HomeRadius);

    //Send shit like a boss.
    sendData(sockfd, l_Buffer, l_MessageSize);

    //Send the Home positions.
    l_MessageSize = 12;
    for(std::list<Team*>::iterator it = Robotix::getInstance()->getFirstTeam(); it != Robotix::getInstance()->getLastTeam(); it++)
    {
        Position* l_Pos = (*it)->getHome()->getPosition();
        pack(l_Buffer, "fff", l_Pos->getX(), l_Pos->getY(), l_Pos->getOrient());
        sendData(sockfd, l_Buffer, l_MessageSize);
    }

    printf("Sent initial config\n");
   
}

void recvAck(int &sockfd)
{
   //Expecting a short int
   int l_ExpectedSize = 4;
   unsigned char l_Buffer[l_ExpectedSize];

    
   recvData(sockfd, l_Buffer, l_ExpectedSize);

   int l_Ack;
   unpack(l_Buffer, "l", &l_Ack);

   printf("Received ACK %d\n", l_Ack);
}

bool isReady()
{
    for (std::map<int,int>::iterator it = m_Clients.begin(); it != m_Clients.end(); it++)
    {
        if ((*it).second != 1)
            return false;
    }
    Game::Robotix::getInstance()->unpause();
    return true;
}

void sendGameInfo(int &sockfd)
{
    using namespace Game;
    int l_MessageSize = 12;
    unsigned char l_Buffer[l_MessageSize];

    //Send the robot positions.
    for(std::vector<Robot*>::iterator it = Robotix::getInstance()->getFirstRobot(); it != Robotix::getInstance()->getLastRobot(); it++)
    {
        Math::Position* l_Pos = (*it)->getPosition();
        pack(l_Buffer, "fff", l_Pos->getX(), l_Pos->getY(), l_Pos->getOrient());

        sendData(sockfd, l_Buffer, l_MessageSize);
    }
    
    //Send the puck positions.
    for(std::list<Puck*>::iterator it = Robotix::getInstance()->getFirstPuck(); it != Robotix::getInstance()->getLastPuck(); it++)
    {
        Math::Position* l_Pos = (*it)->getPosition();
        pack(l_Buffer, "fff", l_Pos->getX(), l_Pos->getY(), l_Pos->getOrient());

        sendData(sockfd, l_Buffer, l_MessageSize);
    }
}

void recvGameInfo(int &sockfd)
{
     using namespace Game;
    int l_MessageSize = 16;
    unsigned char l_Buffer[l_MessageSize];
 
   //Unpacked data goes here. 
    float l_Xpos, l_Ypos, l_Orient;
    int l_Index;

    //Recv robot info.
    int l_TeamPop = Robotix::getInstance()->getRobPop();
    int l_Teams = Robotix::getInstance()->getTeamCount();
    
    for(int i = 0; i <l_TeamPop; i++)
    {
        recvData(sockfd, l_Buffer, l_MessageSize);
        unpack(l_Buffer, "lfff", &l_Index, &l_Xpos, &l_Ypos, &l_Orient);
        if (l_Index < (l_TeamPop*l_Teams))
        {
            Position* l_Pos = Robotix::getInstance()->getRobotAtIndex(l_Index)->getPosition();

            l_Pos->setX(l_Xpos);
            l_Pos->setY(l_Ypos);
            l_Pos->setOrient(l_Orient);
        }
    }

    l_MessageSize = 12;
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

void* ClientLoop(void* sockfd)
{
    int l_Clientfd = (intptr_t)sockfd;
    
    /*
     * 0 = INIT - Send config options, puck, robot, and home locations.
     * 1 = READY
     * 2 = BUSY
     */
    while(1)
    {
        switch(m_Clients[l_Clientfd])
        {
            //Send config options to client.
            case(0):
                sendInitConfig(l_Clientfd);
                recvAck(l_Clientfd);
                m_Clients[l_Clientfd] = 1;
                break;
            //Wait on all clients to be ready.
            //Send game data, and update.
            case(1):
                if (m_Clients.size() == (uint)Game::Robotix::getInstance()->getTeamCount())
                {
                    sendGameInfo(l_Clientfd);
                    m_Clients[l_Clientfd] = 2;
                }
                break;
            //Receive data.
            case(2):
                recvGameInfo(l_Clientfd);
                m_Clients[l_Clientfd] = 1;
                break;
        }
    }
    close(l_Clientfd);
    return 0;
}


