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

    /**
     * Receive 'msgSize' bytes from connection 'conn', into the char buffer 'buffer'.
     */
    int recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize);

    /**
     * Send 'msgSize' bytes from char buffer 'buffer', to connection 'conn'
     */
    int sendWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize);

    /**
     * Pack the 'header' message into the buffer.
     */
    int packHeaderMessage(unsigned char * buffer, Msg_header *header);

    /**
     * Pack the 'header' message into the buffer.
     */
    int packSizeMessage(unsigned char * buffer, Msg_MsgSize *header);

    /**
     * Pack the 'header' message into the buffer.
     */
    int packSensReqMessage(unsigned char * buffer, Msg_RequestSensorData *header);
}

#endif
