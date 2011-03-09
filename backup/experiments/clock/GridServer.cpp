#include "GridServer.h"

using namespace Network;

GridServer::GridServer():Server()
{
    m_uId = -1;
    m_hb_rcvd = 0;
}

int GridServer::handler(int fd)
{
	TcpConnection * l_curConnection = m_Clients[fd];
	if (l_curConnection == NULL)
	{
		return -1; // no such socket descriptor
	}
	Msg_header l_msgHeader;
	
	unsigned char * msg = new unsigned char[20];
	memset(msg, 0, l_msgHeader.size);
	
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
						
				    Msg_uId l_uId;
					memset(msg, 0, l_uId.size);
							
				    printf("Reciving an ID from Controller\n");
					if (l_curConnection->recv(msg, l_uId.size) != 0)
				    {
					    return -1; // recv failed
					}
					unpack(msg, "l", &m_uId);
					printf("Grid has been assigned Id: %u\n", m_uId);
							
				    l_msgHeader.sender = SENDER_GRIDSERVER;
					l_msgHeader.message =  MSG_ACK;
					unsigned char buf[l_msgHeader.size];

					pack(buf, "hh", l_msgHeader.sender, l_msgHeader.message);
                    if (l_curConnection->send(buf, l_msgHeader.size) < 0)
                    {
                        printf("Unable to send ID ACK to Controller\n");
					    return 0;
					}
							
					return 0;
					break;
						
			    default:
					printf("ERROR: no matching msg handler for CONTROLLER\n");
					return 0; // no event handler for this call
					break;
			}
			break;
		case SENDER_CLOCK:
			
            m_hb_rcvd++;
            printf("receiving hb: %i\n", m_hb_rcvd);
			switch (msgType)
			{
		        case MSG_HEARTBEAT:
			        printf("Receiving heart beat\n");
						
					Msg_HB l_hb;
					memset(msg, 0, l_hb.size);
						
					printf("Reciving an HB identifier\n");
					if (l_curConnection->recv(msg, l_hb.size) != 0)
					{
					    return -1; // recv failed
					}
					unpack(msg, "h", &l_hb.hb);
					printf("Performing operations for grid on heartbeat: %u\n", l_hb.hb);
						
					break;
						
				default:
				    printf("ERROR: no matching msg handler for CLOCK\n");
					break;
			}
			break;
			
		default:
			printf("ERROR: no matching msg handler for UNKNOWN\n");
			return 0;
			break;
		
	}
	delete msg;
    return 0;
}


