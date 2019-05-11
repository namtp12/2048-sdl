#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <fstream>
#include "include/Tile.h"
#define BOARD_SIZE 2

using namespace std;

const int SCREEN_WIDTH = 330;
const int SCREEN_HEIGHT = 400;

int board[BOARD_SIZE][BOARD_SIZE];
Tile* tile_board[BOARD_SIZE][BOARD_SIZE];
bool is_new_tile[BOARD_SIZE][BOARD_SIZE];
int dir_line[] = {1, 0, -1, 0};
int dir_col[] = {0, 1, 0, -1};

SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
Tile *bg_texture = NULL;
SDL_Texture* load_texture(string path);
TTF_Font *g_font = NULL;

Tile *score_texture = NULL;
Tile *high_score_textture = NULL;
Tile *game_over_texture = NULL;

bool init();

bool load_media();

bool load_tile();

void close();

void draw();

void new_game();
void showUI();
pair<int, int> gen_unoccupied_pos();

void apply_move(int current_direction);
bool can_do_move(int line, int col, int next_line, int next_col);

void add_piece();
void showUI();

void reset_new_tile();

bool is_board_full();
bool game_over();
int score;
int high_score; //TODO: init this high score by reading from file
bool new_high_score;
int high_score_flag;

string to_string_(int n);

int main(int argc, char* argv[])
{
    //Load high score from file
    ifstream file_in("highscore.txt");
    if(!file_in)
    {
        cout << "Can not find high score file!" << endl;
        exit(1);
    }
    file_in >> high_score;
    file_in.close();

    if(!init())
    {
        cout << "Can not init!" << endl;
        return -1;
    }
    if(!load_media())
    {
        cout << "Can not load media" << endl;
        return -1;
    }
    if(!load_tile())
    {
        cout << "Can not load title" << endl;
        return -1;
    }
    SDL_Event e;
    bool quit = false;
    srand(time(0));
    ofstream file_out("highscore.txt", ios::trunc);
    bool written = false;
    new_game();
    reset_new_tile();
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0 )
        {
            if(e.type == SDL_QUIT)
                quit = true;
            else if(e.type == SDL_KEYDOWN)
            {
                if(game_over() && !written)
                {
                //            cout << high_score_flag;
                    if(high_score_flag) file_out << high_score_flag << endl;
                    else file_out << high_score << endl;
                    written = true;
                }
                switch( e.key.keysym.sym )
                {
                    case SDLK_w:
                    apply_move(2);
                    break;

                    case SDLK_s:
                    apply_move(0);
                    break;

                    case SDLK_a:
                    apply_move(3);
                    break;

                    case SDLK_d:
                    apply_move(1);
                    break;

                    case SDLK_q:
                    if(!written) file_out << high_score << endl;
                    written = true;
                    quit = true;
                    break;

                    case SDLK_n:
                    new_game();
                    written = false;
                    break;

                    default:
                    //
                    break;
                }
            }
        }
        draw();
        showUI();
    }
    file_out.close();
    close();
    return 0;
}

bool init()
{
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Can not init SDL! " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
        g_window = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(g_window == NULL)
        {
            cout << "Can not create window" << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
            if(g_renderer == NULL)
            {
                cout << "Can not create renderer!" << endl;
            }
            else
            {
                SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                if(TTF_Init() == -1)
                {
                    cout << "Can not init ttf " << TTF_GetError() << endl;
                    success = false;
                }
            }
        }
    }
    return success;
}

bool load_media()
{
    bool success = true;
    g_font = TTF_OpenFont("res/calibri.ttf", 28 );
	if( g_font == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
    return success;
}

void close()
{
    bg_texture->free();
    bg_texture = NULL;
    score_texture->free();
    score_texture = NULL;
    game_over_texture->free();
    game_over_texture = NULL;
    high_score_textture->free();
    high_score_textture = NULL;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            tile_board[i][j]->free();
            tile_board[i][j] = NULL;
        }
    }
    TTF_CloseFont( g_font );
    g_font = NULL;
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    g_renderer = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void draw()
{
    SDL_RenderClear(g_renderer);
    bg_texture->render(0, SCREEN_HEIGHT - SCREEN_WIDTH);
//    high_score_textture->render(200, 200);
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            tile_board[i][j]->free();
            SDL_Color color = {0, 0, 0};
            tile_board[i][j]->loadFromRenderedText(to_string_(board[j][i]), color);
            if(board[j][i] > 0)
                tile_board[i][j]->render(45 + 80 * i - tile_board[i][j]->getWidth() / 2,
                                         45 + 80 * j - tile_board[i][j]->getHeight() / 2
                                         + (SCREEN_HEIGHT - SCREEN_WIDTH));
        }
    }
    SDL_RenderPresent(g_renderer);
}

SDL_Texture* load_texture(string path)
{
    SDL_Texture *new_texture = NULL;
    SDL_Surface *init_surface = IMG_Load(path.c_str());
    if(init_surface == NULL)
    {
        cout << "Can not init surface from " << path.c_str() << " " << IMG_GetError();
    }
    else
    {
        new_texture = SDL_CreateTextureFromSurface(g_renderer, init_surface);
        if(new_texture == NULL)
        {
            cout << "Can not create texture from surface " << SDL_GetError() << endl;
        }
        else
        {
            // do nothing here
        }
        SDL_FreeSurface(init_surface);
    }
    return new_texture;
}

bool load_tile()
{
    //Loading success flag
	bool success = true;

	assert(g_font != NULL);
	bg_texture = new Tile(g_renderer, g_font);
	bg_texture->loadFromFile("res/board.bmp");

	high_score_textture = new Tile(g_renderer, g_font);
	score_texture = new Tile(g_renderer, g_font);
    game_over_texture = new Tile(g_renderer, g_font);
	SDL_Color textColor = { 0, 0, 0 };
	high_score_textture->loadFromRenderedText(to_string_(high_score), textColor);
	score_texture->loadFromRenderedText(to_string_(score), textColor);
    game_over_texture->loadFromRenderedText("Game Over!", textColor);
	for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j <BOARD_SIZE; j++)
        {
            tile_board[i][j] = new Tile(g_renderer, g_font);
            tile_board[i][j]->loadFromRenderedText(to_string_(board[i][j]), textColor);
        }
    }
	if( bg_texture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	if(high_score_textture == NULL || score_texture == NULL || game_over_texture == NULL)
    {
        printf("Failed to load texture text!\n");
        success = false;
    }

	return success;
}

void new_game()
{
    if(new_high_score) high_score = score;
    new_high_score = false;
    score = 0;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = 0;
    }
    add_piece();
}

void showUI()
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
    cout << endl << "High score: " << high_score << endl;
    cout << "Score: " << score << endl;
    if(game_over())
    {
        cout << "Game Over!" << endl;
        if(new_high_score)
        {
//            high_score_flag = score; // Put this to game_over();
            cout << "New high score!" << high_score_flag << endl;
        }
    }
//    cout << is_board_full()<< " " << game_over(); // Testing
}

pair<int, int> gen_unoccupied_pos()
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

void apply_move(int direction)
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
    reset_new_tile();
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
                    if(board[next_i][next_j])
                    {
                        score = score + board[next_i][next_j] + board[i][j];
                        is_new_tile[next_i][next_j] = true;
                    }
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

bool can_do_move(int line, int col, int next_line, int next_col)
{
    if(next_line < 0 || next_col < 0 || next_line >= BOARD_SIZE || next_col >= BOARD_SIZE
       || (board[line][col] != board[next_line][next_col] && board[next_line][next_col] != 0)
       || is_new_tile[line][col] || is_new_tile[next_line][next_col])
        return false;
    return true;
}

void add_piece()
{
    pair<int,int> pos = gen_unoccupied_pos();
    int val = (rand() % 2 + 1) * 2;
    board[pos.first][pos.second] = val;
}

bool is_board_full()
{
    for(int i = 0; i < BOARD_SIZE; i++)
        for(int j = 0; j < BOARD_SIZE; j++)
            if(!board[i][j]) return false;
    return true;
}

bool game_over()
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

    if(score > high_score)
    {
        high_score_flag = score; // high score changed, return current high score
//        high_score = score;
        new_high_score = true;
    }
    else
    {
        high_score_flag = 0; // high score not changed
    }
    return true;
}

string to_string_(int num)
{
    ostringstream ss;
    ss << num;
    return ss.str();
}

void reset_new_tile()
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
            is_new_tile[i][j] = false;
    }
}
