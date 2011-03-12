#include "RobotClient.h"
#include "Messages.h"
#include <string.h>
#include "Config.h"

#include <time.h>

using namespace Network;

static time_t init_sec = time(NULL);
static int Timesteps = 0;

RobotClient::RobotClient():Client()
{
}

int RobotClient::sendHeaderMessage(TcpConnection *conn, int sender, int message)
{
    Msg_header l_Header;
    unsigned char l_Buffer[l_Header.size];
    
    l_Header.sender = sender;
    l_Header.message = message;

    if (pack(l_Buffer, Msg_header_format, 
        l_Header.sender, l_Header.message) != l_Header.size)
    {
        DEBUGPRINT("Failed to pack the header message\n");
        return -1;
    }

    //Try to send the header message.
    for (;;)
    {
        if(conn->send(l_Buffer, l_Header.size) == 0)
        {
            DEBUGPRINT("Sent message header.\n");
            break;
        } 
    }
}

int RobotClient::processRobots()
{
    Msg_RequestSensorData l_Req;
    unsigned char l_Buffer[l_Req.size];

    std::vector<int>::const_iterator l_GridEnd = m_Grids.end();
    for (std::vector<int>::const_iterator it = m_Grids.begin(); it != l_GridEnd; it++)
    {
        sendHeaderMessage(m_serverList[(*it)], SENDER_CLIENT, MSG_REQUESTSENSORDATA);        
        
        l_Req.id = 54;
        if (pack(l_Buffer, Msg_RequestSensorData_format, l_Req.id) != l_Req.size)
        {
            DEBUGPRINT("Error packing sensor request message\n");
            return -1;
        }
    
        for (;;)
        {
            if (m_serverList[(*it)]->send(l_Buffer, l_Req.size) == 0)
            {
                DEBUGPRINT("Sent request for sensor data\n");
                break;
            }
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
    unsigned char l_Buffer[69];

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_serverList[fd];
       
    //Receive from the socket into our buffer.
    if (l_Conn->recv(l_Buffer, l_Header.size) < 0)
    {
        DEBUGPRINT("Error receiving header.\n");
        return -1;
    }

    //Unpack the buffer into the 'header' message.
    DEBUGPRINT("unpacking message \n");
    unpack(l_Buffer, Msg_header_format, &l_Header.sender, &l_Header.message); 
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
                    time_t curr_sec = time(NULL); 
                    if (curr_sec > init_sec)
                    {
                        printf("Number of timesteps: %d\n", Timesteps);
                        init_sec = time(NULL);
                        Timesteps = 0;
                   }
                   Timesteps++;

                   DEBUGPRINT("Expecting to receive a heartbeat message from the clock.\n");
                   Msg_HB l_HB;

                    //Receive the heartbeat message.
                    if (l_Conn->recv(l_Buffer+l_Header.size, l_HB.size) == -1)
                    {
                        DEBUGPRINT("Error receiving heartbeat.\n");
                        return -1;
                    }

                    //Unpack heartbeat message into our buffer.
                    unpack(l_Buffer+l_Header.size, Msg_HB_format, &l_HB.hb);
                    DEBUGPRINT("Hearbeat character: %hd\n", l_HB.hb);
                    
                    m_HeartBeat = l_HB.hb; 
                    //
                    //WORK GOES HERE--------------------------------------------------
                    //
                    /*if (processRobots() != 0)
                    {
                        DEBUGPRINT("Error processing robots");
                    }*/

                    l_Header.sender = SENDER_CLIENT;
                    l_Header.message = MSG_HEARTBEAT;
                    pack(l_Buffer, "hhh",l_Header.sender, l_Header.message, l_HB.hb);
                    l_Conn->send(l_Buffer, l_Header.size+l_HB.size);           
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
                        TcpConnection* l_ClockConn = m_serverList[m_ClockFd];
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
                        m_ReadyGrids = 0;
                    }
                     
                }
            }
            break;
    }


    return 0;
}
