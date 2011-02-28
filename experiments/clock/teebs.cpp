#include "GridServer.h"

int main(int argc, char** argv)
{
   /* using namespace Network;
    Server l_Server = Server();
    l_Server.init("12345");
    */
    using namespace Network;
    GridServer l_Server = GridServer();
    l_Server.init(argv[1]);
    l_Server.handler();

    return 0;
}
