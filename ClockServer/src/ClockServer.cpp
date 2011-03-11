#include "ClockServer.h"
#include "Messages.h"
#include "Packer.h"
#include <string.h>

Network::ClockServer::ClockServer()
{
	m_heartbeat = 0;
	m_clientList.empty();
	m_clientMap.empty();
	m_beat = 0;
	m_responded = 0;
}

Network::ClockServer::~ClockServer()
{

}

int Network::ClockServer::handler(int fd)
{
	//printf("handle %i\n", fd);

    //Create a 'header' message and buffer to receive into.
    Msg_header l_Header;
    unsigned char l_Buffer[l_Header.size];
	memset(&l_Header, 0, l_Header.size);
	memset(&l_Buffer, 0, l_Header.size);

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_Clients[fd];
        
    //Receive from the socket into our buffer.
	for (;;)
	{
		if (l_Conn->recv(l_Buffer, l_Header.size) == 0)
		{
			//printf("Recieved Header\n");
			break;
		}
	}
	

    //Unpack the buffer into the 'header' message.
    unpack(l_Buffer, Msg_header_format, &l_Header.sender, &l_Header.message); 
	//printf("Sender: %d Message: %d\n", l_Header.sender, l_Header.message);
    switch(l_Header.sender)
    {
        //Message is from clock.
        case(SENDER_CLIENT):
            switch(l_Header.message)
            {
                //Message is heart beat.
                case(MSG_HEARTBEAT) :
                {
                    //printf("Received a heartbeat from the Client.\n");
                    Msg_HB l_HB;

                    //Receive the heartbeat message.
					for (;;)
					{
						if (l_Conn->recv(l_Buffer, l_HB.size) == 0)
						{
							//printf("Recieved heartbeat\n");
							break;
						}
					}

                    //Unpack heartbeat message into our buffer.
                    unpack(l_Buffer, Msg_HB_format, &l_HB.hb);
                   // printf("Hearbeat character: %hd\n", l_HB.hb);  

                    if (m_beat == l_HB.hb && m_clientMap[fd] == true)
					{
						m_responded++;
						m_clientMap[fd] = false;
					}
					
					if (m_responded == m_servers_total) allConnectionReadyHandler();
                }
            }
            break;
    }

	
    return 0;
}

int Network::ClockServer::handleNewConnection(int fd)
{
	//printf("Adding new Client: %i\n", fd);
	m_clientMap[fd] = false;
	m_clientList.push_back(fd);
	return 0;
}

int Network::ClockServer::allConnectionReadyHandler()
{
	m_responded = 0;
	//printf("All Clients ready\n");
	
	std::vector<int>::const_iterator l_End = m_clientList.end();
	
	int fd;
	int i;
	
	TcpConnection * conn;
	
	Msg_header l_header;
	Msg_HB l_heartBeat;
	
	unsigned char msg[l_header.size];
	
	l_header.sender = SENDER_CLOCK;
	l_header.message = MSG_HEARTBEAT;
	
	if (m_beat == 1)
	{
		m_beat = 0;
	} else {
		m_beat = 1;
	}
	//printf("beat = %u\n", m_beat);
	l_heartBeat.hb = m_beat;
	
	//printf("Total Clients to send Hb: %i\n",m_clientList.size());
	
	for(std::vector<int>::const_iterator it = m_clientList.begin(); it != l_End; it++)
	{
		//printf("Prepairing heartbeat\n");
		i = 0;
		fd = (*it);
		conn = m_Clients[fd];
		
		if (conn == NULL) 
		{
			printf("Conn = NULL\n");
			return -1;
		}
		
		if (pack(msg, Msg_header_format, l_header.sender, l_header.message) != l_header.size)
		{
			return -1; // didn't pack all bytes FAIL & ABORT!!
		}
		
		for (i = 0; i < 10000; i++)
		{
			if (conn->send(msg, l_header.size) == 0) break;
		}
		
		if (i == 100) 
		{
			printf("init failed to send header\n");
			return -1; // failed to send msg
		}
		
		if (pack(msg, Msg_HB_format, l_heartBeat.hb) != l_heartBeat.size)
		{
			return -1; // didn't pack all bytes FAIL & ABORT!!
		}
		
		for (i = 0; i < 10000; i++)
		{
			if (conn->send(msg, l_heartBeat.size) == 0) break;
		}
		
		if (i == 100) 
		{
			printf("init failed to send msg\n");
			return -1; // failed to send msg
		}
		
		//printf("Sent heartbeat %hd\n", l_heartBeat.hb);
		m_clientMap[fd] = true;
	}
	
	
	return 0;
}