#include "Game/Game.h"
#include "GUI/Gui.h"
#include "Network/Client.h"

#include <pthread.h>
#include <string.h>

int main(int argc, char** argv)
{
    
    Server l_Server = {"localhost", "9991"};
    
    InitClient((void*)&l_Server);

    //Client doesn't need graphics.
    //initGraphics(argc, argv);
    return 0;
}
