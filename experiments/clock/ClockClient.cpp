#include "ClockClient.h"
#include "Messages.h"
#include "Packer.h"


Network::ClockClient::ClockClient()
{
	m_heartbeat = 0;
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
	unsigned int x =0;
	while(1)
	{
		//printf("ready for write\n");

		
		
		Msg_header header = {SENDER_CLOCK, MSG_HEARTBEAT};
		unsigned char buf[header.size];

		printf("status of pack %u of %u\n", pack(buf, "hh", header.sender, header.message), header.size);
		//printf("%i: Sending Message %i\n", x,m_heartbeat);
		if (m_conn.send(buf, header.size) < 0) 
		{
			printf("Failed to send header\n");
			return -1;
		}
		Msg_HB heartbeat;
		heartbeat.hb = m_heartbeat;
		unsigned char bufhb[heartbeat.size];
		pack(bufhb, "h", heartbeat.hb);
		printf("%i Sending HB Message %i\n", x,m_heartbeat);
		if (m_conn.send(bufhb, heartbeat.size) < 0) 
		{
			printf("Failed to send heartbeat\n");
			return -1;
		}
		
		if (m_heartbeat == 0) m_heartbeat=1;
		else m_heartbeat=0;
		x++;
	}
	
    return 0;
}
