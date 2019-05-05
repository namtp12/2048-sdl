#include "../include/GameCmd.h"

int board[BOARD_SIZE][BOARD_SIZE];
int board_copy[BOARD_SIZE][BOARD_SIZE];
int dir_line[] = {1, 0, -1, 0};
int dir_col[] = {0, 1, 0, -1};

GameCmd::GameCmd()
{

}

GameCmd::~GameCmd()
{

}

void GameCmd::new_game()
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = 0;
    }
    add_piece();
}

void GameCmd::showUI()
{
    system("cls");
    cout << "n: new game, w: up-1, s: down-0, a: left-3, d: right-2" << endl;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(!board[i][j])
                cout << setw(4) << ".";
            else
                cout << setw(4) << board[i][j];
        }
        cout << endl;
    }
}

pair<int, int> GameCmd::gen_unoccupied_pos()
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

void GameCmd::apply_move(int direction)
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
    int next_i, next_j, move_possible, can_add_piece = 0;
    do
    {
        move_possible = 0;
        for(int i = start_line; 0 <= i && i < 4; i += line_step)
        {
            for(int j = start_col; 0 <=j && j < 4; j += col_step)
            {
                next_i = i + dir_line[direction];
                next_j = j + dir_col[direction];
                if(board[i][j] && can_do_move(i, j, next_i, next_j))
                {
                    board[next_i][next_j] += board[i][j];
                    board[i][j] = 0;
                    move_possible = 1;
                    can_add_piece = 1;
                }
            }
        }
    }
    while(move_possible);
    if(can_add_piece)
        add_piece();
}

bool GameCmd::can_do_move(int line, int col, int next_line, int next_col)
{
    if(next_line < 0 || next_col < 0 || next_line >= 4 || next_col >= 4
       || (board[line][col] != board[next_line][next_col] && board[next_line][next_col] != 0))
        return false;
    return true;
}

void GameCmd::add_piece()
{
    pair<int,int> pos = gen_unoccupied_pos();
    board[pos.first][pos.second] = 2;
}