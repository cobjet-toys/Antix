#include "ControllerClient.h"
#include "Packer.h"
#include "Messages.h"

using namespace Network;

ControllerClient::ControllerClient()
{
}

int ControllerClient::sendId(unsigned int id)
{
   /* //Message header.
    Msg_header l_Header = {SENDER_CONTROLLER, MSG_ASSIGN_ID}; 
    unsigned char l_Buffer[l_Header.size];
    pack(l_Buffer, "hh", l_Header.sender, l_Header.message);
    int l_ResultValue = m_conn.send(l_Buffer, l_Header.size);
    
    //Actual message id.
    Msg_uId l_Id = {id};
    unsigned char l_MsgBuf[l_Id.size];
    pack(l_MsgBuf, "l", l_Id.uId);
	printf("Sending Id %i to Grid Server\n", id);
    l_ResultValue = m_conn.send(l_MsgBuf, l_Id.size);
	if (l_ResultValue < 0) return -1; // failed to send
	
	memset(l_Buffer, 0, l_Header.size);
	l_ResultValue = m_conn.recv(l_MsgBuf, l_Header.size);
	if (l_ResultValue < 0) return -1; // failed to recv
	printf("Recieving Grid Server response\n", id);	
	unpack(l_MsgBuf, "hh", &l_Header.sender, &l_Header.message);
	printf("Unpacking Grid Server response\n", id);
	if (l_Header.message == MSG_ACK) printf("%i \n", l_Header.message);
	
	printf("Sending Robot Information\n");
	printf("Sending P Information\n");	*/
	return 0;
	
}

int ControllerClient::handler()
{
}
