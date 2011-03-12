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
                    Msg_RequestSensorData l_Req;
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
					
					/*if (l_curConnection->recv(l_Buffer, l_Req.size) == 0)
					{
						DEBUGPRINT("Received request for sensor info\n");
						break;
					}
                    

                    //Unpack heartbeat message into our buffer.
                    unpack(l_Buffer, Msg_RequestSensorData_format, &l_Req.id);
                    DEBUGPRINT("Request for object id: %d\n", l_Req.id);
 
                    //Send header.
                    if (pack(l_Buffer, Msg_header_format, 
                        l_Header.sender, l_Header.message) != l_Header.size)
                    {
                        DEBUGPRINT("Failed to pack the header message\n");
                        return -1;
                     }

                    //Try to send the header message.
                    for (;;)
                    {           
                        if(l_curConnection->send(l_Buffer, l_Header.size) == 0)
                        {
                            DEBUGPRINT("Sent message header.\n");
                            break;
                        } 
                    }

                    if (pack(l_Buffer, Msg_RobotSensorData_format, l_RoboData.id, 
                                l_RoboData.xPosition, l_RoboData.yPosition,
                                l_RoboData.hasPuck) != l_RoboData.size)
                    {
                        DEBUGPRINT("Failed to pack the RoboSensor message\n");
                        return -1;
                     }

                    //Try to send the robot sensor message.
                    for (;;)
                    {           
                        if(l_curConnection->send(l_Buffer, l_RoboData.size) == 0)
                        {
                            DEBUGPRINT("Sent RoboSensor message x.\n");
                            break;
                        } 
                    }*/
                break;
            }
            break;
	    case SENDER_CONTROLLER:
		    
            switch (msgType)
			{
		    		
			    case MSG_ASSIGN_ID:
				{
					Msg_header l_msgHeader = {SENDER_GRIDSERVER, MSG_ASSIGN_ID};
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
				}		
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
	//delete msg;	//Not requried, using heap memeory, the program will automatically clean it up when it ends. Unless you use 'new', you should not call delete.
    return 0;
}


