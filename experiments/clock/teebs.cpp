#include "ControllerClient.h"
#include <vector>

int main(int argc, char** argv)
{
    using namespace Network;

    std::vector<ControllerClient*> l_GridServers;

    std::vector<char*> l_GridIPs;

    l_GridIPs.push_back("127.0.0.1");

    /*TODO: FILL UP HOSTS FILE WITH GRID SERVER IPS*/

    std::vector<char*>::const_iterator l_HostsEnd = l_GridIPs.end();  
    for (std::vector<char*>::const_iterator it = l_GridIPs.begin(); it != l_HostsEnd; it++)
    {
        ControllerClient *l_NewClient =  new ControllerClient();
        l_NewClient->init((*it), argv[1]);
        l_GridServers.push_back(l_NewClient);
    } 

    printf("Grid Server connection established\n");
    return 0;
}
