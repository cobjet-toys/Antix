#include "TCPInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int sendAll(int sockfd, unsigned char* msg, int msgSize)
{
    int l_Total = 0, l_BytesLeft = msgSize, l_Sent = 0;

    while(l_Total < l_BytesLeft)
    {
        l_Sent = send(sockfd, msg+l_Total, l_BytesLeft, 0);
        //Check for error.
        if (l_Sent == -1)
            break;
        l_Total += l_Sent;
        l_BytesLeft -= l_Sent;
    }
    
    return l_Sent==-1?-1:0;
}

int rcvAll(int sockfd, unsigned char* msg, int msgSize)
{
    int l_Total = 0, l_BytesLeft = msgSize, l_Rcvd = 0;

    while(l_Total < l_BytesLeft)
    {
        l_Rcvd = recv(sockfd, msg+l_Total, l_BytesLeft, 0);
        //Check for error.
        if (l_Rcvd == -1)
            break;
        l_Total += l_Rcvd;
        l_BytesLeft -= l_Rcvd;
    }
    
    return l_Rcvd==-1?-1:0;
}


//Initialize our connection.
void* InitServerConnection(void* args)
{
    //Common variables.
    ConnectArgs* l_Args = (ConnectArgs*)args;
    //Need to initialize or compiler complains.
    int l_Sockfd = 0; 
    struct addrinfo l_ConHints, *l_ServInfo;
    struct addrinfo l_ConHints, *l_ServInfo;
    struct addrinfo l_ConHints, *l_ServInfo;

    memset(&l_ConHints, 0, sizeof l_ConHints);
    l_ConHints.ai_family = AF_UNSPEC;
    l_ConHints.ai_socktype = SOCK_STREAM;
    l_ConHints.ai_flags = AI_PASSIVE;  

    int l_RetValue;
    if ((l_RetValue = getaddrinfo(l_Args->IP, l_Args->Port, &l_ConHints, &l_ServInfo)) != 0){
        fprintf(stderr, "getaddrinfo %s\n", gai_strerror(l_RetValue));
        exit(1); 
    }

    //Connection iterator.
    struct addrinfo *l_Iter;
    int l_Yes = 1;

    for(l_Iter = l_ServInfo; l_Iter != NULL; l_Iter = l_Iter->ai_next) {
        if ((l_Sockfd = socket(l_Iter->ai_family, l_Iter->ai_socktype, l_Iter->ai_protocol)) == -1) {
            perror("Log: Trying next addrinfo to create socket");
            continue;
        }

        if (setsockopt(l_Sockfd, SOL_SOCKET, SO_REUSEADDR, &l_Yes, sizeof(int)) == -1) {
            perror("Log: Error setting socket options. Exiting");
            exit(1);
        }

        if (bind(l_Sockfd, l_Iter->ai_addr, l_Iter->ai_addrlen) == -1) {
            close(l_Sockfd);
            perror("Log: Trying next addrinfo to bind socket");
            continue;
        }
        break;
    }

    if (l_Iter == NULL) {
        fprintf(stderr, "Failed to create socket.");
        exit(1);
    }

    if (listen(l_Sockfd, BACKLOG) == -1) {
        perror("Log: Error on socket 'listen'");
        exit(1);
    }

    printf("Socket created successfully, listening on port %s \n", l_Args->Port);

    freeaddrinfo(l_ServInfo);

    l_Args->Main((void*)l_Sockfd); 
    
    return (void*)0;
}


//Initialize our Client connection.
void* InitClientConnection(void* args)
{
    //Common variables.
    ConnectArgs* l_Args = (ConnectArgs*)args;
    //Need to initialize or compiler complains.
    int l_Sockfd = 0; 
    struct addrinfo l_ConHints, *l_ServInfo;

    memset(&l_ConHints, 0, sizeof l_ConHints);
    l_ConHints.ai_family = AF_UNSPEC;
    l_ConHints.ai_socktype = SOCK_STREAM;

    int l_RetValue;
    if ((l_RetValue = getaddrinfo(l_Args->IP, l_Args->Port, &l_ConHints, &l_ServInfo)) != 0){
        fprintf(stderr, "getaddrinfo %s\n", gai_strerror(l_RetValue));
        exit(1); 
    }

    //Connection iterator.
    struct addrinfo *l_Iter;

    for(l_Iter = l_ServInfo; l_Iter != NULL; l_Iter = l_Iter->ai_next) {
        if ((l_Sockfd = socket(l_Iter->ai_family, l_Iter->ai_socktype, l_Iter->ai_protocol)) == -1) {
            perror("Log: Trying next addrinfo to create socket");
            continue;
        }
			
            if (connect(l_Sockfd, l_Iter->ai_addr, l_Iter->ai_addrlen) == -1) {
                close(l_Sockfd);
                perror("Log: Trying next addrinfo to connect.");
                continue;
            }
			
        break;
    }

    if (l_Iter == NULL) {
        fprintf(stderr, "Failed to create socket.");
        exit(1);
    }
	

    printf("Connection established to server %s on port %s \n", l_Args->IP, l_Args->Port);

    freeaddrinfo(l_ServInfo);

    l_Args->Main((void*)l_Sockfd); 
    
    return (void*)0;
}
