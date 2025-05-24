#include "MatchmakerServer.h"
#include <iostream>

int main()
{
    MatchmakerServer Server(9090);
    Server.Start();

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    Server.Stop();
    return 0;
}