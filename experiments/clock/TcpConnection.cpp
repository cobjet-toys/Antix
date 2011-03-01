#include "TcpConnection.h"

TcpConnection::TcpConnection()
{
    m_socketfd = -1;
    m_addrinfo = NULL;
}

TcpConnection::TcpConnection(int socketDescriptor, struct sockaddr_storage thier_addr, socklen_t thier_addr_size)
{
	m_thier_addr_size = thier_addr_size;
	m_thier_info = thier_addr;
    m_socketfd = socketDescriptor;	
    m_addrinfo = NULL;
}

TcpConnection::~TcpConnection()
{
	m_addrinfo = NULL;
}

int TcpConnection::bind(struct addrinfo * addrsock)
{
	m_addrinfo = addrsock;
	if (m_addrinfo == NULL ||  m_socketfd == -1)
	{
		perror("Invalid socket cant listen");
		return -2;
	} 
	
	m_addrinfo = addrsock;
    if (::bind(m_socketfd, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) == -1) {
        perror("Could not bind socket (Server)");
        return -1;
    }
    return 0;
}

int TcpConnection::socket(struct addrinfo * addrsock)
{
	if (addrsock == NULL)
	{
		perror("Invalid socket connect");
		return -2;
	}
	m_addrinfo = addrsock;
	int l_filedesc = -1;
	if ((l_filedesc = ::socket(addrsock->ai_family, addrsock->ai_socktype, addrsock->ai_protocol)) == -1)
	{
		perror("Could not create socket");
		return -1;
	}
	m_socketfd = l_filedesc;
	return 0;
}

int TcpConnection::listen(int amount)
{
	if (m_socketfd == -1)
	{
		perror("Invalid socket cant listen");
		return -2;
	} 
	
    if (::listen(m_socketfd, 5) == -1)
    {
		perror("Could not listen (Server)");
		return -1;
    }
    return 0;
}

TcpConnection * TcpConnection::accept()
{
	if (m_socketfd == -1)
	{
		perror("Invalid addrinfo or socket");
		return NULL;
	} 
	
	struct sockaddr_storage thier_addr;
	
	socklen_t thier_addr_size;
	thier_addr_size = sizeof thier_addr;
	
	int newFileDescriptor = -1; 
	if ((newFileDescriptor = ::accept(m_socketfd, (struct sockaddr * )&thier_addr, &thier_addr_size) ) == -1 )
	{
		perror("Could not accept new connection");
		return NULL;
	} 
	return new TcpConnection(newFileDescriptor, thier_addr, thier_addr_size);
}

int TcpConnection::connect(struct addrinfo * addrsock)
{
	m_addrinfo = addrsock;
	
	if (m_addrinfo == NULL || m_socketfd == -1)
	{
		perror("Invalid addrinfo or socket");
		return -2;
	} 
	
	if (::connect(m_socketfd, addrsock->ai_addr, addrsock->ai_addrlen) == -1)
	{
		perror("Could not connect (Client)");
		return -1;
	}
	return 0;
}

int TcpConnection::send(unsigned char * message, int messageSize)
{
	if (m_socketfd == -1)
	{
		perror("Invalid addrinfo or socket");
		return -2;
	} 
	
    int l_Total = 0, l_BytesLeft = messageSize, l_Sent = 0;

    while(l_Total < l_BytesLeft)
    {
        l_Sent = ::send(m_socketfd, message+l_Total, l_BytesLeft, 0);
        //Check for error.
        if (l_Sent == -1)
            break;
        l_Total += l_Sent;
        l_BytesLeft -= l_Sent;
    }
    
    return l_Sent==-1?-1:0;
}

int TcpConnection::recv(unsigned char * message, int messageSize)
{
	if (m_socketfd == -1)
	{
		perror("Invalid addrinfo or socket");
		return -2;
	} 
	
    int l_Total = 0, l_BytesLeft = messageSize, l_Rcvd = 0;

    while(l_Total < l_BytesLeft)
    {
        l_Rcvd = ::recv(m_socketfd, message+l_Total, l_BytesLeft, 0);
        //Check for error.
        if (l_Rcvd == -1)
            break;
        l_Total += l_Rcvd;
        l_BytesLeft -= l_Rcvd;
    }

    return l_Rcvd==-1?-1:0;	
}

int TcpConnection::close()
{
	::shutdown(m_socketfd, 2);
}

int TcpConnection::getSocketFd()
{
	return m_socketfd;
}
