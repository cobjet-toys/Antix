#include "Game/Game.h"
#include "GUI/Gui.h"

int main(int argc, char** argv)
{
    Game::Robotix::getInstance()->init(argc, argv);
    initGraphics(argc, argv);
    return 0;
}
