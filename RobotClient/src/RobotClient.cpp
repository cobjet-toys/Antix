#include "RobotClient.h"
#include "Messages.h"

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
                    
                    //
                    //WORK GOES HERE
                    //

                    //Create heartbeat message.
                    Msg_HB l_HB;

                    //Pack the hearbeat into the header message buffer.
                    pack(l_Buffer, Msg_HB_format, l_HB.hb);

                    //Send the heartbeat message.
                    l_Conn->send(l_Buffer, l_HB.size); 
                    break;
                }
            }
            break;
        case(SENDER_GRIDSERVER):
            break;
    }


    return 0;
}
