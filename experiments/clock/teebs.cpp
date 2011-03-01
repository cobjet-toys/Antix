#include "ControllerClient.h"
#include <vector>
#include <unistd.h>

using namespace Network;
typedef std::vector<ControllerClient*> CtrlClientVector;

int main(int argc, char** argv)
{
    //Vector of active connections to grid servers.
    CtrlClientVector l_GridServers;

    //Vector of ips of grid server to attempt to connect to.
    std::vector<char*> l_GridIPs;

    //Add an ip.
    l_GridIPs.push_back("142.58.172.169");
    //l_GridIPs.push_back("127.0.0.1");

    /*TODO: FILL UP HOSTS FILE WITH GRID SERVER IPS*/

    std::vector<char*>::const_iterator l_HostsEnd = l_GridIPs.end();  
    for (std::vector<char*>::const_iterator it = l_GridIPs.begin(); it != l_HostsEnd; it++)
    {
        ControllerClient *l_NewClient =  new ControllerClient();
        //Port is specified by CLI arguments.
        int l_ResultValue = l_NewClient->init((*it), argv[1]);
        if (l_ResultValue == 0)
        {
            l_GridServers.push_back(l_NewClient);
            printf("Grid Server connection established\n");
        }
        else
        {
            delete l_NewClient;
        }
    } 

      
    size_t l_NumGrids = l_GridServers.size(); 
    for (unsigned int i = 0; i < l_NumGrids; i++)
    {
        l_GridServers[i]->sendId(0);           
    }
    /*TODO: Initialize server*/ 
    
    return 0;
}
