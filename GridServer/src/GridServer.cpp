#include "GridServer.h"
#include "Config.h"

using namespace Network;

GridServer::GridServer():Server()
{
    m_uId = -1;
    m_hb_rcvd = 0;
}
int GridServer::handleNewConnection(int fd)
{
    return 0;
}

int GridServer::allConnectionReadyHandler()
{
    return 0;
}


int GridServer::handler(int fd)
{
	TcpConnection * l_curConnection = m_Clients[fd];
	
	if (l_curConnection == NULL)
    {
		return -1; // no such socket descriptor
	}
	
	Msg_header l_header;
	
	unsigned char msg[l_header.size];
	
	memset(msg, 0, l_header.size);
	
	uint16_t sender=-1, msgType =-1;
	
	if (l_curConnection->recv(msg, l_header.size) != 0)
	{
		DEBUGPRINT("Failed to recv message\n");
		return -1; // recv failed
	}
	
	DEBUGPRINT("Receiving message from client\n");
	unpack(msg, "hh", &sender, &msgType);
	DEBUGPRINT("%u, %u\n", sender, msgType);
	
	if (sender == -1 || msgType == -1) return -1; // bad messages
	
	switch (sender)
	{
        case SENDER_CLIENT:
			
            switch(msgType)
            {
                case (MSG_REQUESTSENSORDATA):
					
                    Msg_RobotSensorData l_RoboData; //BULLSHIT sensor data
                    Msg_header l_Header = {SENDER_GRIDSERVER, MSG_ROBOTSENSORDATA};
					

					Msg_RequestSizeMsg msgSize;
					
                    unsigned char l_Buffer[l_RoboData.size];
					unsigned char l_msgSizeBuff;
					
                    
					if (l_curConnection->recv(l_msgSizeBuff, msgSize.size))
					{
						DEBUGPRINT("Request for robots failed\n");
						break;
					}
					
					int numRobots;
					
					unpack(l_msgSizeBuff, "l", &numRobots);
					
					
                break;
            }
            break;
			
	    case SENDER_CONTROLLER:
		    
            
		case SENDER_CLOCK:
			
          
			
		default:
			printf("ERROR: no matching msg handler for UNKNOWN\n");
			return 0;
			break;
		
	}
	//delete msg;	//Not requried, using heap memeory, the program will automatically clean it up when it ends. Unless you use 'new', you should not call delete.
    return 0;
}


