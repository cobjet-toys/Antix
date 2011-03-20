#include "TcpConnection.h"
#include "networkCommon.h"
#include "Packer.h"
//#include "Config.h"
#include <string.h>


int NetworkCommon::requestHeader(uint16_t &sender, uint16_t &message, TcpConnection * curConnection)
{
	if (curConnection == NULL) return -2; // make sure no bad connection
	sender=-1; message =-1;
	
	
	Msg_header l_header;
	
	unsigned char msg[l_header.size];
	
	memset(msg, 0, l_header.size);
	
	
	if (curConnection->recv(msg, l_header.size) != 0)
	{
		DEBUGPRINT("Failed to recv message\n");
		return -1; // recv failed
	}
	
	DEBUGPRINT("Receiving message from client\n");
	
	unpack(msg, Msg_header_format, &sender, &message); // @todo need some error checking
		
	DEBUGPRINT("%d, %d\n", sender, message);
	
	return 0;
}

int NetworkCommon::packHeader(char * msgBuff, int sender, int message)
{
	
	return 0;
}

int NetworkCommon::requestMessageSize(Msg_MsgSize &msg, TcpConnection * curConnection)
{
	if (curConnection == NULL) return -2; // make sure no bad connection
	
	Msg_MsgSize l_msgSize;
	
	unsigned char l_msgSizeBuff[l_msgSize.size];
                    
	if (curConnection->recv(l_msgSizeBuff, l_msgSize.size) == -1)
	{
		DEBUGPRINT("Request for robots failed\n");
		return -1;
	}

	unpack(l_msgSizeBuff, Msg_MsgSize_format, &msg.msgSize); // @todo need some error checking
	
	return 0;
}

int NetworkCommon::packSizeOfMessage(char * msgBuff, int size)
{
	return 0;
}
