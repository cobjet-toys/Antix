#ifndef NETWORKC_H_
#define NETWORKC_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXDATASIZE 1000

typedef struct{
    char destination[INET6_ADDRSTRLEN];
    char port[5];
} Server;

void* InitClient(void* server);

#endif
