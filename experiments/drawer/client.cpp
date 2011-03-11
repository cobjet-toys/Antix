#include "DrawClient.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    Network::DrawClient * dCli = new Network::DrawClient();
    
    //fake extract drawer address from server.info
    string dbhost = argc == 1? "localhost" : argv[1];
    int homes = argc == 1? 1 : atoi(argv[2]);
    int pucks = argc == 1? 5 : atoi(argv[3]);
    int homePop = argc == 1? 10 : atoi(argv[4]);
    dCli->init(dbhost, homes, pucks, homePop);
    
    dCli->update();

    return (EXIT_SUCCESS);
}

