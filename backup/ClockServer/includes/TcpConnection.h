#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h> 

#define SIZE sizeof(struct sockaddr_in)
#define MAX_ACCEPT_CONNECTIONS 5
#define MAX_PORT_LENGTH 6

class TcpConnection {

	public:

		/*
		 * Default constructor
		 */
		TcpConnection();
		/*
		 * Default constructor with options
		 */
		TcpConnection(int socketDescriptor, struct sockaddr_storage thier_addr, socklen_t thier_addr_size);
		/*
		 * Destructor
		 */
		~TcpConnection();
		
		/*
		 * @brief Used to accept incoming connections.
		 * Needs to have a valid socket connection and 
		 * a bind() and listen() as a pre-req.
		 */
		TcpConnection * accept();
		
		/*
		 * @brief Used to close the file descriptor associated 
		 * with this object
		 */
		int close();
		
		/*
		 * @brief Used to connect to a server. Needs to have a valid 
		 * socket connection before the call.
		 * use getaddrinfo to generate the adderinfo struct "addrsock"
		 */
		int connect(struct addrinfo * addrsock);
		
		/*
		 * @brief Used to create a socket descriptor. Use getaddrinfo to generate the addrinfo.
		 */
		int socket(struct addrinfo * addrsock);
		/*
		 * @brief Used to bind your server to a port.
		 * use getaddrinfo to generate the adderinfo struct "addrsock"
		 */
		int bind(struct addrinfo * addrsock);
		
		/*
		 * @brief Used to listen to incoming connections
		 * defaults to MAX_ACCEPT_CONNECTIONS connections if amount not provided
		 */
		int listen(int amount = MAX_ACCEPT_CONNECTIONS);
		
		/*
		 * @brief Used to send a message through the socket descriptor 
		 * Needs a valid socket descriptor
		 */
		int send(unsigned char * message, int messageSize);
		
		/*
		 * @brief recieve a message through the socket descriptor 
		 * Needs a valid socket descriptor
		 */
		int recv(unsigned char * message, int messageSize);

		/*
		 * @brief Used to get the socket descriptor associated with this object.
		 */
		int getSocketFd();
		
	private:
		
		struct sockaddr_storage m_thier_info; // store information about a client
		socklen_t m_thier_addr_size; // maybe needed
		struct addrinfo * m_addrinfo; // used for connections
		int m_socketfd; // socket descriptor

};

#endif

