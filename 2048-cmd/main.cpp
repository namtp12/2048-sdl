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

void add_piece();

int main()
{
    srand(time(0));
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
            apply_move(current_direction);
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
    add_piece();
}

void showUI()
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(!board[i][j])
                cout << ".";
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
        if(board[line][col] == 0)
            occupied = 0;
    }
//    assert(0 <= line && line <= 3);
//    assert(0 <= col && col <= 3);
    return make_pair(line, col);
}

void apply_move(int direction)
{
    int start_line = 0, start_col = 0, line_step = 1, col_step = 1;
    if(direction == 0)
    {
        start_line = 3;
        line_step = -1;
    }
    if(direction == 1)
    {
        start_col = 3;
        col_step = -1;
    }
    int next_i, next_j, move_possible = 0;
    for(int i = start_line; 0 <= i && i < 4; i += line_step)
    {
        for(int j = start_col; 0 <=j && j < 4; j += col_step)
        {
            next_i = i + dir_line[direction];
            next_j = j + dir_col[direction];
            if(can_do_move(i, j, next_i, next_j))
            {
                board[next_i][next_j] += board[i][j];
                board[i][j] = 0;
                move_possible = 1;
            }
        }
    }
    if(move_possible)
        add_piece();
}

bool can_do_move(int line, int col, int next_line, int next_col)
{
    if(next_line < 0 || next_col < 0 || next_line >= 4 || next_col >= 4
       || board[line][col] != board[next_line][next_col] && board[next_line][next_col] != 0)
        return false;
    return true;
}

void add_piece()
{
    pair<int,int> pos = gen_unoccupied_pos();
    board[pos.first][pos.second] = 2;
}
