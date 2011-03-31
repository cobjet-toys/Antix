#include "ControllerClient.h"
#include "Messages.h"
#include <string.h>
#include "Config.h"
#include <map>
#include <vector>
#include "TcpConnection.h"
#include "networkCommon.h"

#ifdef DEBUG
#include <time.h>
#endif

using namespace Network;

#ifdef DEBUG
static time_t init_sec = time(NULL);
static int Timesteps = 0;
#endif

ControllerClient::ControllerClient():Client()
{
	m_totalGrids = 0;
	m_totalRobotClients = 0;
	m_totalGridsReady = 0;
	m_totalRobotClientsReady = 0;
	m_curRange = 0;
	m_totalGridsPending = 0;
	m_homeRadius = 0.0f;
    m_worldSize = 0.0f;
	m_numGrids = 0;
    m_puckTotal = 0;	
}

int ControllerClient::initGrid(const char* host, const char* port)
{
	m_totalGrids++;
	
    int l_GridFd = initConnection(host, port);

    if (l_GridFd < 0)
    {
        DEBUGPRINT("CONTROLLER_CLIENT: FAILED: Creating connection to %s:%s\n", host, port);
		return -1;
    }
    m_Grids.push_back(l_GridFd);

    strncpy(m_GridConInfo[l_GridFd].first, host, sizeof(m_GridConInfo[l_GridFd].first));
    strncpy(m_GridConInfo[l_GridFd].second, port, sizeof(m_GridConInfo[l_GridFd].second));

//    m_GridConInfo[l_GridFd] = std::make_pair (host, port);
    DEBUGPRINT("Adding grid with ip %s & port %s\n", m_GridConInfo[l_GridFd].first, m_GridConInfo[l_GridFd].second);

	TcpConnection * l_con = m_serverList[l_GridFd];
	if (l_con == NULL) 
	{
		DEBUGPRINT("CONTROLLER_CLIENT: FAILED:\t Cannot init connection to %s:%s, connection.\n", host, port);
		return -1;
	}
	Msg_header l_header;
	Msg_GridId l_gridId;
	
	Msg_WorldInfo l_worldInfo;
	l_worldInfo.homeRadius = m_homeRadius; 
    l_worldInfo.worldSize = m_worldSize;
	l_worldInfo.numGrids = m_numGrids;
    l_worldInfo.puckTotal = m_puckTotal;	
	
	unsigned char l_headerBuff[l_header.size+l_gridId.size+l_worldInfo.size];
	
	if (NetworkCommon::packHeader(l_headerBuff, SENDER_CONTROLLER, MSG_REQUESTGRIDWAITING) < 0)
	{
		DEBUGPRINT("CONTROLLER_CLIENT: FAILED:\t Cannot pack ready header to %s:%s, connection.\n", host, port);
		return -1;
	}
	int l_offset = l_header.size;
	if (pack(l_headerBuff+l_offset, Msg_GridId_format, m_totalGrids) != l_gridId.size)
	{
		DEBUGPRINT("CONTROLLER_CLIENT: FAILED:\t failed packing id to %s:%s, connection.\n", host, port);
		return -1;
	}
	
	l_offset += l_gridId.size;
	
	if (pack(l_headerBuff+l_offset, Msg_WorldInfo_format, l_worldInfo.homeRadius, l_worldInfo.worldSize, l_worldInfo.numGrids, l_worldInfo.puckTotal) != l_worldInfo.size)
	{
		DEBUGPRINT("CONTROLLER_CLIENT: FAILED:\t failed packing world info to %s:%s, connection.\n", host, port);
		return -1;
	}
	
	if (NetworkCommon::sendMsg(l_headerBuff, l_header.size+l_gridId.size+l_worldInfo.size, l_con) < 0)
	{
		DEBUGPRINT("CONTROLLER_CLIENT: FAILED:\t Cannot send ready header to %s:%s, connection.\n", host, port);
		return -1;
	}
	
	DEBUGPRINT("CONTROLLER_CLIENT: STATUS:\t Ready header sent to GRID %s:%s, connection.\n", host, port);
	
	
    return l_GridFd;
}

int ControllerClient::initNeighbourGrids()
{
    const int l_GridSize = m_Grids.size();
         
    DEBUGPRINT("Found a total of %zu grids\n", m_Grids.size());
    for (int i = 0; i < l_GridSize; i++)
    {
        //Get our nearby grids.
        int l_LeftGrid, l_RightGrid;

        if (i-1 < 0)
        {
            l_LeftGrid = m_Grids.back();
        }
        else
        {
            l_LeftGrid = m_Grids[i-1];
        }
        if (i+1 == l_GridSize)
        {
           l_RightGrid = m_Grids.front();
        }
        else
        {
            l_RightGrid = m_Grids[i+1];
        }
        DEBUGPRINT("CONTROLLER_CLIENT: STATUS\t Have Grid File Descriptors\n");
        //Send the nearby grids.
        Msg_header l_Header;

        Msg_GridNeighbour l_Neighbour;
        l_Neighbour.position = 0;

        strncpy(l_Neighbour.ip, m_GridConInfo[l_LeftGrid].first, sizeof(l_Neighbour.ip));
        strncpy(l_Neighbour.port, m_GridConInfo[l_LeftGrid].second, sizeof(l_Neighbour.port));

        Msg_MsgSize l_NumNeighbours = {2};

        unsigned int l_BuffSize = l_Header.size+l_NumNeighbours.size+(2*l_Neighbour.size);
        unsigned char l_Buffer[l_BuffSize]; 
   
        unsigned int l_Offset = 0; 

        DEBUGPRINT("CONTROLLER_CLIENT: STATUS\t Created neighbour\n");
        //Pack header.
        NetworkCommon::packHeader(l_Buffer+l_Offset, SENDER_CONTROLLER, MSG_GRIDNEIGHBOURS);
        l_Offset += l_Header.size;

        //Pack size.
        pack(l_Buffer+l_Offset, Msg_MsgSize_format, l_NumNeighbours.msgSize);
        l_Offset += l_NumNeighbours.size;

        //Pack left neighbour
        pack(l_Buffer+l_Offset, Msg_gridNeighbour_format, l_Neighbour.position, l_Neighbour.ip, l_Neighbour.port);
        DEBUGPRINT("Packed neighbour with ip %s and port %s\n", l_Neighbour.ip, l_Neighbour.port);
        
        l_Offset += l_Neighbour.size;

        //Prepare and pack the right neighbour
        l_Neighbour.position = 1;
        strncpy(l_Neighbour.ip, m_GridConInfo[l_RightGrid].first, sizeof(l_Neighbour.ip));
        strncpy(l_Neighbour.port, m_GridConInfo[l_RightGrid].second, sizeof(l_Neighbour.port));

        DEBUGPRINT("Packed neighbour with ip %s and port %s\n", l_Neighbour.ip, l_Neighbour.port);

        pack(l_Buffer+l_Offset, Msg_gridNeighbour_format, l_Neighbour.position, l_Neighbour.ip, l_Neighbour.port);

        NetworkCommon::sendMsg(l_Buffer, l_BuffSize, m_serverList[m_Grids[i]]);
    }
}

int ControllerClient::initRobotClient(const char* host, const char* port)
{
    int l_RoboClient = initConnection(host, port);

    if (l_RoboClient < 0)
    {
        DEBUGPRINT("CONTROLLER_CLIENT: FAILED\t creating connection to a grid server\n");
		return -1;
    }
    m_RobotClients.push_back(l_RoboClient);

    return l_RoboClient;
}

int ControllerClient::initClock(const char* host, const char* port)
{
    int l_ClockFd = initConnection(host, port);

    if (l_ClockFd < 0)
    {
        DEBUGPRINT("CONTROLLER_CLIENT: FAILED\t creating connection to a grid server\n");
		return -1;
    }
    m_ClockFd = l_ClockFd;

    return l_ClockFd;
}

int ControllerClient::beginSimulation()
{
    TcpConnection* l_ClockCon = m_serverList[m_ClockFd];

    Msg_header l_Header;

    unsigned int l_MsgSize = l_Header.size;
    unsigned char l_Buffer[l_MsgSize];

    NetworkCommon::packHeader(l_Buffer, SENDER_CONTROLLER, MSG_HEARTBEAT);

    NetworkCommon::sendMsg(l_Buffer, l_MsgSize, l_ClockCon);
}

void ControllerClient::setHomeRadius(float homeRadius)
{
	m_homeRadius = homeRadius;
}

void ControllerClient::setWorldSize(float worldSize)
{
	m_worldSize = worldSize;
}

void ControllerClient::numGrids(int numGrids)
{
	m_numGrids = numGrids;
}

void ControllerClient::numPucksTotal(int pucksTotal)
{
	m_puckTotal = pucksTotal;
}

int ControllerClient::handler(int fd)
{
	DEBUGPRINT("CONTROLLER_CLIENT: STATUS\t Handling file descriptor: %i\n", fd);

    //Create a 'header' message and buffer to receive into.
    Msg_header l_Header;
    unsigned char l_HeaderBuffer[l_Header.size];

    //Get our TCPConnection for this socket.
    TcpConnection *l_Conn =  m_serverList[fd];
        
    //Receive the header message.
    NetworkCommon::recvHeader(l_Header.sender, l_Header.message, l_Conn);

    //Unpack the buffer into the 'header' message.
    //unpack(l_HeaderBuffer, Msg_header_format, &l_Header.sender, &l_Header.message); 
    DEBUGPRINT("CONTROLLER_CLIENT: STATUS\t Received message %d from %d\n", l_Header.message, l_Header.sender);
	
    switch(l_Header.sender)
    {
        //Message is from clock.
        case(SENDER_CLOCK):
            switch(l_Header.message)
            {
              //TODO CLOCK MESSAGES 
            }
            break;
        case(SENDER_GRIDSERVER):
            switch(l_Header.message)
            {
				case (MSG_RESPONDGRIDWAITING):
				{
					Msg_GridId l_gridId;
					Msg_GridRequestIdRage l_reqGridRange;

					
					unsigned char l_id[l_gridId.size + l_reqGridRange.size];

					memset(&l_id, 0, l_gridId.size + l_reqGridRange.size);
					memset(&l_gridId, 0, l_gridId.size);
					memset(&l_reqGridRange, 0, l_reqGridRange.size);

					
					if (l_Conn->recv(l_id, l_gridId.size) < 0)
					{
						DEBUGPRINT("CONTROLLER_CLIENT: FAILED\t to recieve ID\n");
						return -1;
					}

					if (l_Conn->recv(l_id+l_gridId.size, l_reqGridRange.size) < 0)
					{
						DEBUGPRINT("CONTROLLER_CLIENT: FAILED\t to recieve size\n");
						return -1;
					}

					unpack(l_id, "lll", &l_gridId.id, &l_reqGridRange.teams, &l_reqGridRange.robotsPerTeam);
					
					if (l_gridId.id < 0 || l_reqGridRange.teams < 0 || l_reqGridRange.robotsPerTeam < 0)
					{
						DEBUGPRINT("CONTROLLER_CLIENT: FAILED\t Id or Number of Teams or Team Size less than zero\n");
						return -1;
					}
					
					DEBUGPRINT("CONTROLLER_CLIENT: STATUS\t Have response from grid ID %lu Teams = %lu robotsPerTeam = %lu\n", (unsigned long)l_gridId.id, (unsigned long)l_reqGridRange.teams, (unsigned long)l_reqGridRange.robotsPerTeam);
					
					uint32_t totalRobots = l_reqGridRange.teams * l_reqGridRange.robotsPerTeam;
					
					m_curRange+=1;
					
					Msg_RobotIdRange l_range;
					l_range.from = m_curRange;
					l_range.to = m_curRange + totalRobots-1;
					
					m_curRange = l_range.to;
					
					unsigned char l_rangeBuff[l_range.size + l_Header.size];
					
					if (NetworkCommon::packHeader(l_rangeBuff, SENDER_CONTROLLER, MSG_RESPONDGRIDRANGE) < 0)
					{
						DEBUGPRINT("CONTROLLER_CLIENT FAILED:\t Cannot pack ready header to grid.\n");
						return -1;
					}
					
					if (pack(l_rangeBuff+l_Header.size, Msg_RobotIdRange_format, l_range.from, l_range.to) != l_range.size)
					{
						DEBUGPRINT("CONTROLLER_CLIENT FAILED:\t Cannot pack range data for grid\n");
						return -1;
					}
					uint32_t id;

					if (NetworkCommon::sendMsg(l_rangeBuff, l_Header.size+l_range.size, l_Conn) < 0)
					{
						DEBUGPRINT("CONTROLLER_CLIENT FAILED:\t Cannot send grid range data\n");
						return -1;
					}
					
					++m_totalGridsPending;
					
					if (m_totalGrids == m_totalGridsPending)
					{
						initNeighbourGrids();
						
					}
				}	
				break;
					
				case(MSG_GRIDCONFIRMSTARTED):
				{
					Msg_GridId l_gridId;
					unsigned char message[l_gridId.size];
					
					if (l_Conn->recv(message, l_gridId.size) < 0)
					{
						DEBUGPRINT("CONTROLLER_CLIENT FAILED:\t Did not receive the grid id from complete message.");
						return -1;
					}
					
					unpack(message, Msg_GridId_format, &l_gridId.id);
					
					DEBUGPRINT("CONTROLLER_CLIENT STATUS:\t Grid Id %lu has responded with READY\n", (unsigned long)l_gridId.id);
					
					++m_totalGridsReady;
					DEBUGPRINT("gridReady=%u, gridstotal=%u\n", m_totalGridsReady, m_totalGrids);
					if (m_totalGridsReady == m_totalGrids)
					{
						DEBUGPRINT("CONTROLLER_CLIENT STATUS:\t ALL GRIDS + CLIENTS READY\n");
						DEBUGPRINT("CONTROLLER_CLIENT STATUS:\t Can now send information to clock\n");

						Msg_header l_header;
						unsigned char l_buffer[l_header.size];
						
						if (NetworkCommon::packHeader(l_buffer, SENDER_CONTROLLER, MSG_CLOCKPROCEED) < 0)
						{
							return -1;
						}

						TcpConnection * l_con = m_serverList[m_ClockFd];
						
						if (NetworkCommon::sendMsg(l_buffer,l_header.size,l_con) < 0)
						{
							return -1;
						}

					}
					
				}
				break;
				
            }
            break;
    }
    return 0;
}

