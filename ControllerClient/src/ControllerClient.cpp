#include "ControllerClient.h"
#include "Messages.h"
#include <string.h>
#include "Config.h"
#include <map>
#include <vector>

#ifdef DEBUG
#include <time.h>
#endif

using namespace Network;

#ifdef DEBUG
static time_t init_sec = time(NULL);
static int Timesteps = 0;
#endif

ControllerClient::ControllerClient():Client()
{
}

int ControllerClient::sendWrapper(TcpConnection * conn, unsigned char* buffer, int msgSize)
{
    if (conn->send(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to send a message.\n");
        return -1;
    }
    return 0;
}

int ControllerClient::recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize)
{
    if (conn->recv(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to receive a message.\n");
        return -1;
    }
    return 0;
}

int ControllerClient::packHeaderMessage(unsigned char* buffer, uint16_t sender, uint16_t message)
{
    Msg_header l_Header;

    l_Header.sender = sender;
    l_Header.message = message;

    if (pack(buffer, Msg_header_format, l_Header.sender, l_Header.message) != l_Header.size)
    {
        DEBUGPRINT("Failed to pack a header message. Sender: %d, Message: %d", sender, message);
        return -1;
    }
    return 0;
}
int ControllerClient::initGrid(const char* host, const char* port)
{
    int l_GridFd = initConnection(host, port);

    if (l_GridFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
    }
    m_Grids.push_back(l_GridFd);
    m_GridConInfo[l_GridFd] = std::make_pair (host, port);

    return l_GridFd;
}

int ControllerClient::initNeighbourGrids()
{
    const int l_GridSize = m_Grids.size();
         
    for (int i = 0; i < l_GridSize; i++)
    {
        //Get our nearby grids.
        int l_LeftGrid, l_RightGrid;

        if (i-1 < 0)
        {
            l_LeftGrid = m_Grids.back();
        }
        else
        {
            l_LeftGrid = m_Grids[i-1];
        }
        if (i+1 == l_GridSize)
        {
           l_RightGrid = m_Grids.front();
        }
        else
        {
            l_RightGrid = m_Grids[i+1];
        }
        //Send the nearby grids.
        Msg_header l_Header;

        Msg_GridNeighbour l_Neighbour;
        l_Neighbour.position = 0;
        strncpy(l_Neighbour.ip, m_GridConInfo[m_Grids[l_LeftGrid]].first, sizeof(l_Neighbour.ip));
        strncpy(l_Neighbour.port, m_GridConInfo[m_Grids[l_LeftGrid]].second, sizeof(l_Neighbour.port));

        Msg_MsgSize l_NumNeighbours = {2};

        unsigned int l_BuffSize = l_Header.size+l_NumNeighbours.size+(2*l_Neighbour.size);
        unsigned char l_Buffer[l_BuffSize]; 
   
        unsigned int l_Offset = 0; 
        //Pack header.
        packHeaderMessage(l_Buffer+l_Offset, SENDER_CONTROLLER, MSG_GRIDNEIGHBOURS);
        l_Offset += l_Header.size;

        //Pack size.
        pack(l_Buffer+l_Offset, Msg_MsgSize_format, l_NumNeighbours.msgSize);
        l_Offset += l_NumNeighbours.size;

        //Pack left neighbour
        pack(l_Buffer+l_Offset, Msg_gridNeighbour_format, l_Neighbour.position, l_Neighbour.ip, l_Neighbour.port);
        
        l_Offset += l_Neighbour.size;

        //Prepare and pack the right neighbour
        l_Neighbour.position = 1;
        strncpy(l_Neighbour.ip, m_GridConInfo[m_Grids[l_RightGrid]].first, sizeof(l_Neighbour.ip));
        strncpy(l_Neighbour.port, m_GridConInfo[m_Grids[l_RightGrid]].second, sizeof(l_Neighbour.port));

        pack(l_Buffer+l_Offset, Msg_gridNeighbour_format, l_Neighbour.position, l_Neighbour.ip, l_Neighbour.port);

        sendWrapper(m_serverList[m_Grids[i]], l_Buffer, l_BuffSize);
    }
}

int ControllerClient::initRobotClient(const char* host, const char* port)
{
    int l_RoboClient = initConnection(host, port);

    if (l_RoboClient < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
    }
    m_RobotClients.push_back(l_RoboClient);

    return l_RoboClient;
}

int ControllerClient::initClock(const char* host, const char* port)
{
    int l_ClockFd = initConnection(host, port);

    if (l_ClockFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
    }
    m_ClockFd = l_ClockFd;

    return l_ClockFd;
}

int ControllerClient::beginSimulation()
{
    TcpConnection* l_ClockCon = m_serverList[m_ClockFd];

    Msg_header l_Header;
    Msg_HB l_hb = {0};

    unsigned int l_MsgSize = l_hb.size+l_Header.size;
    unsigned char l_Buffer[l_MsgSize];

    packHeaderMessage(l_Buffer, SENDER_CONTROLLER, MSG_HEARTBEAT);
}

int ControllerClient::handler(int fd)
{
	DEBUGPRINT("Handling file descriptor: %i\n", fd);

    //Create a 'header' message and buffer to receive into.
    Msg_header l_Header;
    unsigned char l_HeaderBuffer[l_Header.size];

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_serverList[fd];
        
    //Receive the header message.
    recvWrapper(l_Conn, l_HeaderBuffer, l_Header.size);

    //Unpack the buffer into the 'header' message.
    unpack(l_HeaderBuffer, Msg_header_format, &l_Header.sender, &l_Header.message); 
    DEBUGPRINT("Received message %d from %d\n", l_Header.message, l_Header.sender);
    switch(l_Header.sender)
    {
        //Message is from clock.
        case(SENDER_CLOCK):
            switch(l_Header.message)
            {
              //TODO CLOCK MESSAGES 
            }
            break;
        case(SENDER_GRIDSERVER):
            switch(l_Header.message)
            {
                //TODO GRID MESSAGES
            }
            break;
        case(SENDER_CLIENT):
            switch(l_Header.message)
            {
                //TODO CLIENT MESSAGES
            }
            break;
    }
    return 0;
}

