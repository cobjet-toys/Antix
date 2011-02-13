#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
//Required for memset
#include <string.h>

//Connection arguments.
typedef struct {
    //Destination IP.
    char IP[INET6_ADDRSTRLEN];
    //Destination port.
    char Port[6];
    //Main function loop.
    void* (*Main)(void*);
} ConnectArgs;

#define BACKLOG 10

void* InitConnection(void* args);
int sendAll(int sockfd, unsigned char* msg, int msgSize);
int rcvAll(int sockfd, unsigned char* msg, int msgSize);
