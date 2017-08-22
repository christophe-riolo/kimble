#include <string>
#include <iostream>
#include "kimble.hpp"

using std::cout;
using std::endl;
using std::stoi;

int main(int argc, char* argv[])
{
    Game* game = NULL;
    if(argc == 1)
        game = new Game();
    else if (argc == 2)
        game = new Game(stoi(argv[1]));
    else
    {
        cout << "Too many arguments." << endl << "Usage : kimble [<number of players<]." << endl;
        return 1;
    }
    
    game->play();
    delete game;
    return 0;
}
