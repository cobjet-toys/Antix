#include "GridServer.h"
#include "Config.h"
#include "Types.h"
#include <map>
#include <vector>

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
	if (unpack(msg, "hh", &sender, &msgType) != l_header.size)
	{
		DEBUGPRINT("Could not unpack message header.\n");
		return 0;
	}
	DEBUGPRINT("%u, %u\n", sender, msgType);
	
	if (sender == -1 || msgType == -1) return -1; // bad messages
	
	switch (sender)
	{
        case SENDER_CLIENT:
			
            switch(msgType)
            {
                case (MSG_REQUESTSENSORDATA):
					
					Msg_RequestSizeMsg msgSize;
					
					unsigned char l_msgSizeBuff[msgSize.size];
					
                    
					if (l_curConnection->recv(l_msgSizeBuff, msgSize.size) == -1)
					{
						DEBUGPRINT("Request for robots failed\n");
						return -1;
					}
					
					int numRobots=0;
					
					if (unpack(l_msgSizeBuff, Msg_RequestSizeMsg_format, &numRobots) != msgSize.size)
					{
						DEBUGPRINT("Numbur of robots could not be determined\n");
						return -1;
					}
					
					if (numRobots <= 0)
					{
						DEBUGPRINT("Requested sensor data for <=0 Robots. Game error.");
						return -1;
					}
					
					DEBUGPRINT("Client has requested %i robot's sensor data: ", %numRobots);
					
					vector<int> robot_ids;
					robot_ids.erase();
					
					Msg_RequestSensorData l_robotId;
					unsigned char l_robotIdBuff[l_robotId.size * numRobots];
					
					
					for (int i = 0; i < numRobots; i++)
					{
						if (l_curConnection->recv(l_robotIdBuff+(l_robotId.size * i), l_robotId.size) == -1)
						{
							DEBUGPRINT("Could not get robot id for returnSensorData\n");
							return 0;
						}
						unsigned int id;
						if (unpack(l_robotIdBuff+(l_robotId.size * i),Msg_robotSensorRequestRobotId_format, &id) != l_robotId.size)
						{
							return -1;
						}
						robot_ids.push_back(id);
					}

					

					//gridGameInstance.returnSensorData(vector<unsigned int>* robot_ids, map<unsigned int, vector<sensed_item> >* sensed_items_map);


					

					std::map<int, vector<sensed_item> > * sensed_items_map = new std::map<int, vector<sensed_item> >;
					
					//std::map<int>::const_iterator it = 
					
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


