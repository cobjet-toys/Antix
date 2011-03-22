#ifndef __NETWORK_COMMON_H__
#define __NETWORK_COMMON_H__

#include "Messages.h"

class TcpConnection;

namespace NetworkCommon
{

	int recvHeader(uint16_t &sender, uint16_t &message, TcpConnection * curConnection);
	int packHeader(unsigned char * buffer, int sender, int message);
	int sendMsg(unsigned char* buffer, int msgSize, TcpConnection * conn);
	
	int recvMessageSize(Msg_MsgSize &msg, TcpConnection * curConnection);
	int packSizeOfMessage(unsigned char * buffer, int size);

}

#endif