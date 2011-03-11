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

int RobotClient::processRobots()
{
    Msg_RequestSensorData l_Req;
    unsigned char l_Buffer[l_Req.size];

    std::vector<int>::const_iterator l_GridEnd = m_Grids.end();
    for (std::vector<int>::const_iterator it = m_Grids.begin(); it != l_GridEnd; it++)
    {
        l_Req.id = 123456;
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

int RobotClient::handler(int fd)
{
    time_t curr_sec = time(NULL); 
    if (curr_sec > init_sec)
    {
        printf("Number of timesteps: %d", Timesteps);
        init_sec = time(NULL);
        Timesteps = 0;
    }
    Timesteps++;

	DEBUGPRINT("Handling file descriptor: %i\n", fd);

    //Create a 'header' message and buffer to receive into.
    Msg_header l_Header;
    unsigned char l_Buffer[l_Header.size];

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_serverList[fd];
        
    //Receive from the socket into our buffer.
    for(;;)
    {
        if (l_Conn->recv(l_Buffer, l_Header.size) == 0)
        {
            DEBUGPRINT("Received Header\n");
            break;
        }
    }

    //Unpack the buffer into the 'header' message.
    unpack(l_Buffer, Msg_header_format, &l_Header.sender, &l_Header.message); 

    switch(l_Header.sender)
    {
        //Message is from clock.
        case(SENDER_CLOCK):
            switch(l_Header.message)
            {
                //Message is heart beat.
                case(MSG_HEARTBEAT) :
                {
                    DEBUGPRINT("Expecting to receive a heartbeat message from the clock.\n");
                    Msg_HB l_HB;

                    //Receive the heartbeat message.
                    for(;;)
                    {
                        if (l_Conn->recv(l_Buffer, l_HB.size) == 0)
                        {
                            DEBUGPRINT("Received hearbeat\n");
                            break;
                        }

                    }

                    //Unpack heartbeat message into our buffer.
                    unpack(l_Buffer, Msg_HB_format, &l_HB.hb);
                    DEBUGPRINT("Hearbeat character: %hd\n", l_HB.hb);
                     
                    //
                    //WORK GOES HERE--------------------------------------------------
                    //
                    processRobots();

                    //Prepare our 'header' message.
                    //Set the robot client as the sender
                    l_Header.sender = SENDER_CLIENT; 

                    //Set the message type as header.
                    l_Header.message = MSG_HEARTBEAT;

                    //Pack the sender and messege into the header message buffer.
                    if (pack(l_Buffer, Msg_header_format, 
                                l_Header.sender, l_Header.message) != l_Header.size)
                    {
                        DEBUGPRINT("Failed to pack the header message\n");
                        return -1;
                    }

                    //Try to send the header message.
                    for (;;)
                    {
                        if(l_Conn->send(l_Buffer, l_Header.size) == 0)
                        {
                            DEBUGPRINT("Sent message header.\n");
                            break;
                        } 
                    }
                   
                    //Pack the hearbeat into the header message buffer.
                    if (pack(l_Buffer, Msg_HB_format, l_HB.hb) != l_HB.size)
                    {
                        DEBUGPRINT("Failed to pack the HB message\n");
                        return -1;
                    }

                    //Try to send the heartbeat message 100x.
                    for (;;)
                    {
                        if(l_Conn->send(l_Buffer, l_HB.size) == 0)
                        {
                            DEBUGPRINT("Sent heartbeat.\n");
                            break;
                        } 
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
                    }
            }
            break;
    }


    return 0;
}
