#include "RobotClient.h"
#include "Messages.h"
#include <string.h>
#include "Config.h"

#ifdef DEBUG
#include <time.h>
#endif

using namespace Network;

#ifdef DEBUG
static time_t init_sec = time(NULL);
static int Timesteps = 0;
#endif

RobotClient::RobotClient():Client()
{
}

int RobotClient::sendWrapper(TcpConnection * conn, unsigned char* buffer, int msgSize)
{
    if (conn->send(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to send a message.\n");
        return -1;
    }
    return 0;
}

int RobotClient::recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize)
{
    if (conn->recv(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to receive a message.\n");
        return -1;
    }
    return 0;
}

int RobotClient::packHeaderMessage(unsigned char* buffer, int sender, int message)
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

int RobotClient::sendRobotRequests()
{
    //Create our request messages
    Msg_RequestSensorData l_Req;
    Msg_header l_Header;
    Msg_RequestSizeMsg l_Size;

    //For each grid, send a message request containing all of the robot ids for that grid.
    std::vector<int>::const_iterator l_GridEnd = m_Grids.end();
    for (std::vector<int>::const_iterator it = m_Grids.begin(); it != l_GridEnd; it++)
    {
        l_Size.sizeOfMsg = 10;//REPLACE WITH ACTUAL REQUEST FOR ROBOTS

        unsigned int l_MessageSize = l_Size.sizeOfMsg;
        unsigned char l_Buffer[l_MessageSize];
        packHeaderMessage(l_Buffer, SENDER_CLIENT, MSG_REQUESTSENSORDATA);        
        
        l_Req.id = 54;
        for (int i = 0; i < l_Size.sizeOfMsg;i++)
        {

        }
            
    } 
    return 0;
}

int RobotClient::initGrid(const char* host, const char* port)
{
    int l_GridFd = initConnection(host, port);

    if (l_GridFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
    }
    m_Grids.push_back(l_GridFd);

    return l_GridFd;
}

int RobotClient::initClock(const char* host, const char* port)
{
    int l_ClockFd = initConnection(host, port);

    if (l_ClockFd < 0)
    {
        DEBUGPRINT("Failed creating connection to a grid server\n");
    }
    m_ClockFd = l_ClockFd;

    return l_ClockFd;
}

int RobotClient::handler(int fd)
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
                //Message is heart beat.
                case(MSG_HEARTBEAT) :
                {
#ifdef DEBUG
                    time_t curr_sec = time(NULL); 
                    if (curr_sec > init_sec)
                    {
                        DEBUGPRINT("Number of timesteps: %d", Timesteps);
                        init_sec = time(NULL);
                        Timesteps = 0;
                   }
                   Timesteps++;
#endif
                   DEBUGPRINT("Expecting to receive a heartbeat message from the clock.\n");

                   Msg_HB l_HB;
                   unsigned char l_hbBuffer[l_HB.size]; 
                  
                   recvWrapper(l_Conn, l_hbBuffer, l_HB.size); 

                   //Unpack heartbeat message into our buffer.
                   unpack(l_hbBuffer, Msg_HB_format, &l_HB.hb);
                   DEBUGPRINT("Hearbeat character: %hd\n", l_HB.hb);
                    
                   m_HeartBeat = l_HB.hb; 
                   if (sendRobotRequests() != 0)
                   {
                       DEBUGPRINT("Error sending robot sensor requests\n");
                   }
               }
            }
            break;
        case(SENDER_GRIDSERVER):
            switch(l_Header.message)
            {
                case(MSG_ROBOTSENSORDATA) :
                {
                    //
                    //PROCESS SENSOR DATA
                    //
                    Msg_RobotSensorData l_RoboData;
                    unsigned char l_LargeBuffer[l_RoboData.size];              
                     //Receive the robo sensor message.
                    for(;;)
                    {
                        if (l_Conn->recv(l_LargeBuffer, l_RoboData.size) == 0)
                        {
                            DEBUGPRINT("Received robo sensor data\n");
                            break;
                        }

                    }

                    //Unpack heartbeat message into our buffer.
                    unpack(l_LargeBuffer, Msg_RobotSensorData_format,
                            &l_RoboData.id, &l_RoboData.xPosition,
                            &l_RoboData.yPosition, &l_RoboData.hasPuck);
                    DEBUGPRINT("ID: %d, XPOS: %f, YPOS: %f, hasPuck: %c\n", l_RoboData.id, 
                                l_RoboData.xPosition, l_RoboData.yPosition,
                                    l_RoboData.hasPuck);
                    

                    m_ReadyGrids++; 
                    DEBUGPRINT("Recevied sensory data from a grid\n");
                    if (m_ReadyGrids == m_Grids.size())
                    {   
                        /*TcpConnection* l_ClockConn = m_serverList[m_ClockFd];
                        //Prepare our 'header' message.
                        sendHeaderMessage(l_ClockConn, SENDER_CLIENT, MSG_HEARTBEAT);
                     
                        Msg_HB l_HB = {m_HeartBeat};

                        //Pack the hearbeat into the header message buffer.
                        if (pack(l_Buffer, Msg_HB_format, l_HB.hb) != l_HB.size)
                        {
                            DEBUGPRINT("Failed to pack the HB message\n");
                            return -1;
                        }

                        //Try to send the heartbeat message 100x.
                        for (;;)
                        {
                            if(l_ClockConn->send(l_Buffer, l_HB.size) == 0)
                            {
                                DEBUGPRINT("Sent heartbeat.\n");
                                break;
                            } 
                        }
                        m_ReadyGrids = 0;*/
                    }
                     
                }
            }
            break;
    }


    return 0;
}

