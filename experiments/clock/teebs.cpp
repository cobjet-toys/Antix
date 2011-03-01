#include "ControllerClient.h"
#include <vector>

int main(int argc, char** argv)
{
    typedef std::vector<char[INET6_ADDRSTRLEN]> HostVector;
    HostVector l_Hosts;

    /*TODO: FILL UP HOSTS FILE WITH GRID SERVER IPS*/

    HostVector::const_iterator l_HostsEnd = l_Hosts.end();  
    for (HostVector::const_iterator it = l_Hosts.begin(); it != l_HostsEnd; it++)
    {
        
    } 
    return 0;
}
