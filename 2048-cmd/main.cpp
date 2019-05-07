#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cassert>
#include <fstream>
#include "include/GameCmd.h"
#include "include/Game.h"
#include <SDL2/SDL.h>

using namespace std;

//int high_score_flag;

int main(int argc, char* args[])
{
    int high_score;
    bool written = false;
    ifstream file_in("highscore.txt");
    if(!file_in)
    {
        cout << "Can not find high score file!" << endl;
        exit(1);
    }
    file_in >> high_score;
    file_in.close();
    ofstream file_out("highscore.txt", ios::trunc);
    GameCmd g(high_score);
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
        if(g.game_over() && !written)
        {
//            cout << high_score_flag;
            if(high_score_flag) file_out << high_score_flag << endl;
            else file_out << high_score << endl;
            written = true;
        }
        g.showUI();
        cin >> command;
        if(command == 'n')
        {
            g.new_game();
            written = false;
        }
        else if(command == 'q')
        {
            if(!written) file_out << high_score << endl;
            break;
        }
        else
        {
            current_direction = commandToDir[(int)command];
            g.apply_move(current_direction);
//            cout << current_direction << endl;
        }
    }
//    cout << high_score_flag; //Testing
    file_out.close();
    return 0;
}
