#include "ControllerClient.h"
#include "Packer.h"
#include "Messages.h"

using namespace Network;

ControllerClient::ControllerClient()
{
}

int ControllerClient::sendId(int id)
{
    Msg_header l_TestHeader = {SENDER_CONTROLLER, MSG_ASSIGN_ID}; 
    
    unsigned char l_Buffer[l_TestHeader.size];

    pack(l_Buffer, "hh", l_TestHeader.sender, l_TestHeader.message);

    m_conn.send(l_Buffer, l_TestHeader.size);
}

int ControllerClient::handler()
{
}
