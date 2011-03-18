#ifndef __NETWORK_COMMON_H__
#define __NETWORK_COMMON_H__

#include "Messages.h"

class TcpConnection;

namespace NetworkCommon
{

	int requestHeader(uint16_t &sender, uint16_t &message, TcpConnection * curConnection);
	int packHeader(char * msgBuff, int sender, int message);
	int requestMessageSize(Msg_MsgSize &msg, TcpConnection * curConnection);
	int packSizeOfMessage(char * msgBuff, int size);
	
}

#endif