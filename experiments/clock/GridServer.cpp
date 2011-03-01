#include "GridServer.h"

using namespace Network;

GridServer::GridServer():Server()
{
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
	int sender=-1, msgType =-1;
	
	if (l_curConnection->recv(msg, (int)l_msgHeader.size) != 0)
	{
		return -1; // recv failed
	}
	
	unpack(msg, "hh", &sender, &msgType);
	
	if (sender == -1 || msgType == -1) return -1; // bad messages
	
	switch (sender)
	{
			case SENDER_CONTROLLER:
				
				switch (msgType)
				{
					
					case MSG_ASSIGN_ID:
						
						break;
						
					default:
						
						break;
				}
				
				break;
		
		
	}
}


