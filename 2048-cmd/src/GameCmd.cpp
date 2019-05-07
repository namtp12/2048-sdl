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
    if(game_over()) cout << "Game Over!" << endl;
//    cout << is_board_full()<< " " << game_over(); // Testing
}

pair<int, int> GameCmd::gen_unoccupied_pos()
{
    int line, col;
    int occupied = 1;
    while(occupied)
    {
        line = rand() % BOARD_SIZE;
        col = rand() % BOARD_SIZE;
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
        start_line = BOARD_SIZE - 1;
        line_step = -1;
    }
    if(direction == 1)
    {
        start_col = BOARD_SIZE - 1;
        col_step = -1;
    }
    int next_i, next_j, move_possible, can_add_piece = 0;
    do
    {
        move_possible = 0;
        for(int i = start_line; 0 <= i && i < BOARD_SIZE; i += line_step)
        {
            for(int j = start_col; 0 <=j && j < BOARD_SIZE; j += col_step)
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
    if(next_line < 0 || next_col < 0 || next_line >= BOARD_SIZE || next_col >= BOARD_SIZE
       || (board[line][col] != board[next_line][next_col] && board[next_line][next_col] != 0))
        return false;
    return true;
}

void GameCmd::add_piece()
{
    pair<int,int> pos = gen_unoccupied_pos();
    int val = (rand() % 2 + 1) * 2;
    board[pos.first][pos.second] = val;
}

bool GameCmd::is_board_full()
{
    for(int i = 0; i < BOARD_SIZE; i++)
        for(int j = 0; j < BOARD_SIZE; j++)
            if(!board[i][j]) return false;
    return true;
}

bool GameCmd::game_over()
{
    if(!is_board_full()) return false;
    // Check if any of 2 adjacent tile is equal
    if(board[BOARD_SIZE - 1][BOARD_SIZE - 1] == board[BOARD_SIZE - 2][BOARD_SIZE - 1] ||
       board[BOARD_SIZE - 1][BOARD_SIZE - 1] == board[BOARD_SIZE - 1][BOARD_SIZE - 2])
       return false;
    for(int i = 0; i < BOARD_SIZE - 1; i++)
        for(int j = 0; j < BOARD_SIZE - 1; j++)
            if(board[i][j] == board[i + 1][j] || board[i][j] == board[i][j + 1])
                return false;
    return true;
}
