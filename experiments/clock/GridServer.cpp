#include "GridServer.h"

using namespace Network;

GridServer::GridServer():Server()
{
	m_uId = -1;
}

int GridServer::handler(int fd)
{
	TcpConnection * l_curConnection = m_Clients[fd];
	if (l_curConnection == NULL)
	{
		return -1; // no such socket descriptor
	}
	Msg_header l_msgHeader;
	
	unsigned char * msg = new unsigned char[l_msgHeader.size];
							Msg_uId l_uId;
							unsigned char * l_uIdBuf = new unsigned char[l_uId.size];
	uint16_t sender=-1, msgType =-1;
	
	if (l_curConnection->recv(msg, (int)l_msgHeader.size) != 0)
	{
		return -1; // recv failed
	}
	
	printf("Reciving message from client\n");
	
	unpack(msg, "hh", &sender, &msgType);
	printf("%u, %u\n", sender, msgType);
	if (sender == -1 || msgType == -1) return -1; // bad messages
	
	switch (sender)
	{
			case SENDER_CONTROLLER:
				
				switch (msgType)
				{
					
					case MSG_ASSIGN_ID:
							
							printf("Reciving an ID from Controller\n");
							if (l_curConnection->recv(l_uIdBuf, l_uId.size) != 0)
							{
								return -1; // recv failed
							}
							unpack(l_uIdBuf, "l", &m_uId);
							printf("Grid has been assigned Id: %u\n", m_uId);
							
							return 0;
						break;
						
					default:
						return 0;
						break;
				}
				
				break;
			case SENDER_CLOCK:
				switch (msgType)
				{
					case MSG_HEARTBEAT:
						printf("Recieing heart beat\n");
						break;
				}
				
				break;
			
			default:
				return 0;
				break;
		
		
	}
	
	delete msg;
}


