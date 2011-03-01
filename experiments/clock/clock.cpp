#include <stdio.h>
#include "ClockClient.h"

int main(int argc, char* argv[])
{
    //Create clock server, waiting for information about grid servers

    //Create clock client, create connection with grid servers
    Network::ClockClient c;
    c.init(argv[1], argv[2]);
    printf("waa\n");
    return 0;
}
