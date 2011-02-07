#include "Game/Game.h"
#include "GUI/Gui.h"

int main(int argc, char** argv)
{
    Game::Robotix::getInstance()->init();
    initGraphics(argc, argv);
    return 0;
}
