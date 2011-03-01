#include "ClockClient.h"
#include "Messages.h"
#include "Packer.h"


Network::ClockClient::ClockClient()
{

}

Network::ClockClient::~ClockClient()
{

}

int Network::ClockClient::handler()
{
    return 0;
}

int Network::ClockClient::heartbeat()
{
    Msg_header header = {SENDER_CLOCK, MSG_HEARTBEAT};
    unsigned char buf[header.size];

    pack(buf, "hh", header.sender, header.message);
    m_conn.send(buf, header.size);
    return 0;
}
