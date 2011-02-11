#include "Game/Game.h"
#include "GUI/Gui.h"
#include "Network/DummyServer.h"

#include <pthread.h>

int main(int argc, char** argv)
{
    const char* l_Port = "9991";
    pthread_t l_Network;
    pthread_create(&l_Network, NULL, InitNetwork, (void*)l_Port);


    Game::Robotix::getInstance()->init();
    initGraphics(argc, argv);
    return 0;
}
