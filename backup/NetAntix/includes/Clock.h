#ifndef CLOCK_H
#define CLOCK_H

#include "Client.h"
#include "Server.h"

class Clock: public Network::Server
{
	Clock();
	~Clock();
	
	int handler();
	/* Derived Members
	 * Client();
	 * int init(char * host, char * port);
	 * virtual ~Client();
	 * virtual int handler() = 0;
	 */
};

#endif