#include "ControllerClient.h"
#include "Packer.h"
#include "Messages.h"

using namespace Network;

ControllerClient::ControllerClient()
{
}

int ControllerClient::sendId(unsigned int id)
{
    //Message header.
    Msg_header l_Header = {SENDER_CONTROLLER, MSG_ASSIGN_ID}; 
    unsigned char l_Buffer[l_Header.size];
    pack(l_Buffer, "hh", l_Header.sender, l_Header.message);
    int l_ResultValue = m_conn.send(l_Buffer, l_Header.size);
    
    //Actual message id.
    Msg_uId l_Id = {id};
    unsigned char l_MsgBuf[l_Id.size];
    pack(l_MsgBuf, "l", l_Id.uId);
    l_ResultValue = m_conn.send(l_MsgBuf, l_Id.size);
}

int ControllerClient::handler()
{
}
