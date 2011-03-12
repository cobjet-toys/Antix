#include "ClockServer.h"
#include "Messages.h"
#include "Packer.h"
#include <string.h>
#include "Config.h"

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
	//Create a 'header' message and buffer to receive into.
	Msg_header l_Header;
	Msg_HB l_heartBeat;
	unsigned char l_Buffer[l_Header.size + l_heartBeat.size];

	memset(&l_Buffer, 0, l_Header.size+l_heartBeat.size);

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_Clients[fd];
        
    //Receive from the socket into our buffer.
	DEBUGPRINT("About to receive header\n");
	if (l_Conn->recv(l_Buffer, l_Header.size) == -1)
	{
		printf("Could not receive\n");
		return -1;
	}

    unpack(l_Buffer, Msg_header_format, &l_Header.sender, &l_Header.message); 
	DEBUGPRINT("Sender: %d Message: %d\n", l_Header.sender, l_Header.message);
    switch(l_Header.sender)
    {
        //Message is from clock.
        case(SENDER_CLIENT):
            switch(l_Header.message)
            {
                //Message is heart beat.
                case(MSG_HEARTBEAT) :
                {
					DEBUGPRINT("Receiving heartbeat from robot client\n");
					if (l_Conn->recv(l_Buffer+l_Header.size, l_heartBeat.size) == -1)
					{
						DEBUGPRINT("Could not receive\n");
						return -1;
					}
					
					DEBUGPRINT("Unpack the heartbeat\n");
					unpack(l_Buffer+l_Header.size, Msg_HB_format, &l_heartBeat.hb);
					
					DEBUGPRINT("Heartbeat character is %d", l_heartBeat.hb);
                    if (m_beat == l_heartBeat.hb && m_clientMap[fd] == true)
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
	DEBUGPRINT("Adding new Client: %i\n", fd);
	m_clientMap[fd] = false;
	m_clientList.push_back(fd);
	return 0;
}

int Network::ClockServer::allConnectionReadyHandler()
{
	m_responded = 0;
	DEBUGPRINT("All Clients ready\n");
	
	std::vector<int>::const_iterator l_End = m_clientList.end();
	
	int fd;
	int i;
	
	TcpConnection * conn;
	
	Msg_header l_header;
	Msg_HB l_heartBeat;
	
	unsigned char msg[l_header.size + l_heartBeat.size];
	
	l_header.sender = SENDER_CLOCK;
	l_header.message = MSG_HEARTBEAT;
	
	if (m_beat == 1)
	{
		m_beat = 0;
	} else {
		m_beat = 1;
	}
	DEBUGPRINT("beat = %u\n", m_beat);
	l_heartBeat.hb = m_beat;
	
	DEBUGPRINT("Total Clients to send Hb: %i\n",m_clientList.size());

	for(std::vector<int>::const_iterator it = m_clientList.begin(); it != l_End; it++)
	{
		DEBUGPRINT("Prepairing heartbeat\n");
		i = 0;
		fd = (*it);
		conn = m_Clients[fd];
		
		if (conn == NULL) 
		{
			printf("Conn = NULL\n");
			return -1;
		}
		int l_packed ;
		if ((l_packed = pack(msg, "hhh", l_header.sender, l_header.message, l_heartBeat.hb )) != l_header.size+l_heartBeat.size)
		{
			return -1; // didn't pack all bytes FAIL & ABORT!!
		}
		printf("%i packed\n", l_packed);

		if (conn->send(msg, l_header.size+l_heartBeat.size) == 0);
		
		DEBUGPRINT("Sent heartbeat %hd\n", l_heartBeat.hb);
		m_clientMap[fd] = true;
	}
	
	
	return 0;
}