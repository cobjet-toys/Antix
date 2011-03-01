#include "Client.h"

int main(int argc, char** argv)
{
    using namespace Network;

    Client aClient = Client();
    aClient.init(argv[1], argv[2]); 
    return 0;
}
