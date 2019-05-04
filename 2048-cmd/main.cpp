#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cassert>
#include "include/GameCmd.h"
#include "include/Game.h"
#include <SDL2/SDL.h>

using namespace std;

Game g;

int main(int argc, char* args[])
{
    srand(time(0));
    char command, commandToDir[256];
    int current_direction;
    commandToDir['s'] = 0;
    commandToDir['d'] = 1;
    commandToDir['w'] = 2;
    commandToDir['a'] = 3;

    g.new_game();
    while(1)
    {
        g.showUI();
        cin >> command;
        if(command == 'n')
            g.new_game();
        else if(command == 'q')
            break;
        else
        {
            current_direction = commandToDir[(int)command];
            g.apply_move(current_direction);
//            cout << current_direction << endl;
        }
    }
    return 0;
}
