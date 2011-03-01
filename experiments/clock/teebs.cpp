#include "ControllerClient.h"
#include <vector>

typedef std::vector<ControllerClient*> CtrlClientVector;

int main(int argc, char** argv)
{
    using namespace Network;

    //Vector of active connections to grid servers.
    CtrlClientVector l_GridServers;

    //Vector of ips of grid server to attempt to connect to.
    std::vector<char*> l_GridIPs;

    //Add an ip.
    l_GridIPs.push_back("127.0.0.1");

    /*TODO: FILL UP HOSTS FILE WITH GRID SERVER IPS*/

    std::vector<char*>::const_iterator l_HostsEnd = l_GridIPs.end();  
    for (std::vector<char*>::const_iterator it = l_GridIPs.begin(); it != l_HostsEnd; it++)
    {
        ControllerClient *l_NewClient =  new ControllerClient();
        //Port is specified by CLI arguments.
        l_NewClient->init((*it), argv[1]);
        l_GridServers.push_back(l_NewClient);
    } 

    printf("Grid Server connection established\n");
    
    /*TODO: Send id to grids*/
    CtrlClientVector::const_iterator l_HostsEnd = l_GridIPs.end();  
    for (CtrlClientVector::const_iterator it = l_GridIPs.begin(); it != l_HostsEnd; it++)
    {
               
    } 



    return 0;
}
