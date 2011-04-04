#include "TcpConnection.h"
#include "networkCommon.h"
#include "Packer.h"
#include "Config.h"
#include <string.h>
#include "Messages.h"

int NetworkCommon::sendWrapper(TcpConnection * conn, unsigned char* buffer, int msgSize)
{
    if (conn->send(buffer, msgSize) == -1)
    {
        ERRORPRINT("Failed to send a message.\n");
        return -1;
    }
    return 0;
}

int NetworkCommon::recvWrapper(TcpConnection* conn, unsigned char* buffer, int msgSize)
{
    if (conn->recv(buffer, msgSize) == -1)
    {
        ERRORPRINT("Failed to receive a message.\n");
        return -1;
    }
    return 0;
}

int NetworkCommon::packHeaderMessage(unsigned char * buffer, Msg_header *header)
{
    unsigned int l_Packed = 0;

    l_Packed = pack(buffer, Msg_header_format, header->sender, header->message);

    if (l_Packed != header->size)
    {
        ERRORPRINT("Failed to pack a header message. Sender: %d, Message: %d\n", header->sender, header->message);
        return -1;
    }
    return l_Packed;	
}

int NetworkCommon::packSizeMessage(unsigned char * buffer, Msg_MsgSize *size)
{
    unsigned int l_Packed = 0;

    l_Packed = pack(buffer, Msg_MsgSize_format, size->msgSize);

    if (l_Packed != size->size)
    {
        ERRORPRINT("Failed to pack a size message%d.\n", size->msgSize);
        return -1;
    }
    return l_Packed;	
}

int NetworkCommon::packSensReqMessage(unsigned char * buffer, Msg_RequestSensorData *sens)
{
    unsigned int l_Packed = 0;

    l_Packed = pack(buffer, Msg_RequestSensorData_format, sens->id);
    if (l_Packed != sens->size)
    {
        ERRORPRINT("Failed to pack a sensory request%d.\n", sens->id);
        return -1;
    }
    return l_Packed;	
}

int NetworkCommon::recvHeader(uint16_t &sender, uint16_t &message, TcpConnection * curConnection)
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
	
	//DEBUGPRINT("Receiving message from client\n");
	
	unpack(msg, Msg_header_format, &sender, &message); // @todo need some error checking
		
	//DEBUGPRINT("%d, %d\n", sender, message);
	
	return 0;
}

int NetworkCommon::packHeader(unsigned char * buffer, int sender, int message)
{
	Msg_header l_Header;

    l_Header.sender = sender;
    l_Header.message = message;

    if (pack(buffer, Msg_header_format, l_Header.sender, l_Header.message) != l_Header.size)
    {
        DEBUGPRINT("Failed to pack a header message. Sender: %d, Message: %d", sender, message);
        return -1;
    }
    return 0;	
}

int NetworkCommon::sendMsg(unsigned char* buffer, int msgSize, TcpConnection * curConnection)
{
	if (curConnection->send(buffer, msgSize) == -1)
    {
        DEBUGPRINT("Failed to send a message.\n");
        return -1;
    }
    return 0;
}

int NetworkCommon::recvMessageSize(Msg_MsgSize &msg, TcpConnection * curConnection)
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

int NetworkCommon::recvMessageSize32(Msg_MsgSize_32 &msg, TcpConnection * curConnection)
{
	if (curConnection == NULL) return -2; // make sure no bad connection
	
	Msg_MsgSize_32 l_msgSize;
	
	unsigned char l_msgSizeBuff[l_msgSize.size];
                    
	if (curConnection->recv(l_msgSizeBuff, l_msgSize.size) == -1)
	{
		DEBUGPRINT("Request for robots failed\n");
		return -1;
	}

	unpack(l_msgSizeBuff, Msg_MsgSize_32_format, &msg.msgSize); // @todo need some error checking

	return 0;
}

int NetworkCommon::packSizeOfMessage(unsigned char * buffer, int size)
{
	return 0;
}
