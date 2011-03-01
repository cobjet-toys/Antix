#include "ControllerClient.h"

int main(int argc, char** argv)
{
    using namespace Network;
    ControllerClient l_CtrlClient = ControllerClient();
    l_CtrlClient.init(argv[1], argv[2]);
 
    return 0;
}
