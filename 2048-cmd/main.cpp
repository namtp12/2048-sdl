#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cassert>

using namespace std;

#define BOARD_SIZE 4

int board[BOARD_SIZE][BOARD_SIZE];
int board_copy[BOARD_SIZE][BOARD_SIZE];
int dir_line[] = {1, 0, -1, 0};
int dir_col[] = {0, 1, 0, -1};


void new_game();
void showUI();
pair<int, int> gen_unoccupied_pos();

void apply_move(int current_direction);
bool can_do_move(int line, int col, int next_line, int next_col);

int main()
{
    char command, commandToDir[256];
    int current_direction;
    commandToDir['s'] = 0;
    commandToDir['d'] = 1;
    commandToDir['w'] = 2;
    commandToDir['a'] = 3;

    while(1)
    {
        showUI();
        cin >> command;
        if(command == 'n')
            new_game();
        else if(command == 'q')
            break;
        else
        {
            current_direction = commandToDir[(int)command];
//            cout << current_direction << endl;
        }
    }
    return 0;
}

void new_game()
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = 0;
    }
    pair<int,int> pos = gen_unoccupied_pos();
    board[pos.first][pos.second] = 2;
}

void showUI()
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(!board[i][j])
                cout << " ";
            else
                cout << board[i][j];
        }
        cout << endl;
    }
    cout << "n: new game, w: up-1, s: down-0, a: left-3, d: right-2" << endl;
}

pair<int, int> gen_unoccupied_pos()
{
    int line, col;
    int occupied = 1;
    while(occupied)
    {
        line = rand() % 4;
        col = rand() % 4;
        if(board[line][occupied] == 0)
            occupied = 0;
    }
//    assert(0 <= line && line <= 3);
//    assert(0 <= col && col <= 3);
    return make_pair(line, col);
}

void apply_move(int direction)
{
    int start_line = 0, start_col = 0, line_step = 1, col_step = 1;
    switch(direction)
    {
    case 0:

    }
}

bool can_do_move(int line, int col, int next_line, int next_col)
{
    if(next_line < 0 || next_col < 0 || next_line >= 4 || next_col >= 0
       || (board[line][col] !board[next_line][next_col] && board[next_line][next_col] != 0))
        return false;
    return true;
}
