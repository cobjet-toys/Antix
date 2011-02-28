#ifndef CLIENT_H
#define CLIENT_H

#include <netdb.h>
#include <string.h>
#include "TcpConnection.h"

namespace Network
{
	
	class client
	{
	public:
		client();
		int init(char * host, char * port);
		~client();
		
		virtual int handler() = 0;
		
	private:
		char * m_host;
		char *  m_port;
		struct addrinfo m_info;
		TcpConnection m_conn;
	};

}

#endif