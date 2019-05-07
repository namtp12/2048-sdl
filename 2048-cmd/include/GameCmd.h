#ifndef GAMECMD_H
#define GAMECMD_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <cassert>

using namespace std;

#define BOARD_SIZE 2

extern int board[BOARD_SIZE][BOARD_SIZE];
extern int board_copy[BOARD_SIZE][BOARD_SIZE];
extern int dir_line[];
extern int dir_col[];


extern int high_score_flag;

class GameCmd
{
public:


    GameCmd();
    GameCmd(int high_score);
    virtual ~GameCmd();


    void new_game();
    void showUI();
    pair<int, int> gen_unoccupied_pos();

    void apply_move(int current_direction);
    bool can_do_move(int line, int col, int next_line, int next_col);

    void add_piece();

    bool is_board_full();
    bool game_over();
protected:
private:
    int score;
    int high_score;
    bool new_high_score;
    fstream file;
};

#endif // GAMECMD_H
