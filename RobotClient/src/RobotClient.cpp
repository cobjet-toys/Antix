#include "RobotClient.h"
#include "Messages.h"
#include <string.h>

using namespace Network;

RobotClient::RobotClient():Client()
{
	m_heartbeat = 0;
}

int RobotClient::handler(int fd)
{
	printf("Handling file descriptor: %i\n", fd);

    //Create a 'header' message and buffer to receive into.
    Msg_header l_Header;
    unsigned char l_Buffer[l_Header.size];

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_serverList[fd];
        
    //Receive from the socket into our buffer.
    l_Conn->recv(l_Buffer, l_Header.size);

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
                    printf("Received a heartbeat from the clock.\n");
                    Msg_HB l_HB;

                    //Receive the heartbeat message.
                    while (l_Conn->recv(l_Buffer, l_HB.size) != 0)
                    {
                        printf("waiting to rcv\n");
                    }

                    //Unpack heartbeat message into our buffer.
                    unpack(l_Buffer, Msg_HB_format, &l_HB.hb);
                    printf("Hearbeat character: %hd\n", l_HB.hb);
                     
                    //
                    //WORK GOES HERE--------------------------------------------------
                    //

                    //Prepare our 'header' message.
                    //Set the robot client as the sender
                    l_Header.sender = SENDER_CLIENT; 

                    //Set the message type as header.
                    l_Header.message = MSG_HEARTBEAT;

                    //Pack the sender and messege into the header message buffer.
                    if (pack(l_Buffer, Msg_header_format, 
                                &l_Header.sender, &l_Header.message) != l_Header.size)
                    {
                        printf("Failed to pack the header message\n");
                        return -1;
                    }

                    //Try to send the header message 100x.
                    int i ;
                    for (i = 0; i < 100; i++)
                    {
                        if(l_Conn->send(l_Buffer, l_Header.size) == 0)
                        {
                            printf("Sent message header.\n");
                            break;
                        } 
                    }

                    if (i == 100)
                    {
                        printf("Couldn't send the header.\n");
                        return -1;
                    }
                    
                    //Prepare to send heartbeat.
                    //Set heartbeat.
                    l_HB.hb = 9;
                   
                    //Pack the hearbeat into the header message buffer.
                    if (pack(l_Buffer, Msg_HB_format, l_HB.hb) != l_HB.size)
                    {
                        printf("Failed to pack the HB message\n");
                        return -1;
                    }

                    //Try to send the heartbeat message 100x.
                    for (i = 0; i < 100; i++)
                    {
                        if(l_Conn->send(l_Buffer, l_HB.size) == 0)
                        {
                            printf("Sent heartbeat.\n");
                            break;
                        } 
                    }

                    if (i == 100)
                    {
                        printf("Couldn't send the heartbeat.\n");
                        return -1;
                    }
                }
            }
            break;
        case(SENDER_GRIDSERVER):
            break;
    }


    return 0;
}
