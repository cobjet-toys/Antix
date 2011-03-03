#include <stdio.h>
#include "ClockClient.h"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Please supply an IP address and port to connect to.\n");
    }
    //Create clock server, waiting for information about grid servers

    //Create clock client, create connection with grid servers
    Network::ClockClient c;
    int i = c.init(argv[1], argv[2]);
    printf("%i\n", i);
    if(i < 0)
    {
        printf("init failed\n.");
        return -1;
    }
    c.heartbeat();
    return 0;
}
