#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Pack_Unpack.h"

#define DEBUG true
#define SIZE sizeof(struct sockaddr_in)

int newSockFd;

int main(int args, char ** argv)
{
	int sockfd;
	pid_t processId;
	int32_t id;
	float32_t xpos, ypos;
	unsigned char buff[1024];
	
	in_addr_t serverIp;
	serverIp = inet_addr("127.0.0.1");
	
	struct sockaddr_in server = {
		AF_INET,
		9701,
		serverIp
		};
	
	// get a socket to work with
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		if (DEBUG) printf("Could not open a socket:\t%i\n",  sockfd);
		exit(0);
	}
	
	// get a binding for a port and socket file descriptor
	if (bind(sockfd, (struct sockaddr *)&server, SIZE) == -1)
	{
		if (DEBUG) printf("Could not bind socket: %i\n", sockfd);		
		exit(0);		
	}
	
	// start listening on the socket descriptor
	if (listen(sockfd, 5) == -1)
	{
		if (DEBUG) printf("listen call failed\n");
		exit(0);		
	}
	
	for ( ; ; )
	{
		if (DEBUG) printf("%u blocking on accept\n", getpid());
		
		// accept new connections 
		if ( (newSockFd = accept(sockfd, NULL, NULL)) == -1)
		{
			if (DEBUG) printf("Could not accept a connection. %i\n", newSockFd);
			return 0;		
		} 
		
		//if (DEBUG) printf("Assigned the file  %i\n", newSockFd);
		
		if (!fork())
		{
			close(sockfd);
			while(recv(newSockFd, &buff, 12, 0) > 0)
			{
				unpack(buff, "lff", &id, &xpos, &ypos);
				printf("\nRecieved Request from Robot:%i XPOS=%f YPOS=%f from child %u", id, xpos, ypos, (unsigned int) getpid());
				++xpos;
				++ypos;
				pack(buff, "lff", id, xpos, ypos);
				send(newSockFd, buff, 12, 0);
				printf("\nSend Response to Robot:%i XPOS=%f YPOS=%f from child %u\n", id, xpos, ypos, (unsigned int) getpid());							
			}
			close(newSockFd);
			return 0;
		}
		
		close(newSockFd);
	}
	return 0;
}
