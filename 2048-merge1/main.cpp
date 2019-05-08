#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iomanip>
#include <sstream>
#include "include/Tile.h"
#define BOARD_SIZE 2

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

int board[BOARD_SIZE][BOARD_SIZE];
Tile* tile_board[BOARD_SIZE][BOARD_SIZE];
int dir_line[] = {1, 0, -1, 0};
int dir_col[] = {0, 1, 0, -1};

SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
Tile *bg_texture = NULL;
SDL_Texture* load_texture(string path);
TTF_Font *g_font = NULL;

Tile *tile1 = NULL;

bool init();

bool load_media();

bool load_title();

void close();

void draw();

void new_game();
void showUI();
pair<int, int> gen_unoccupied_pos();

void apply_move(int current_direction);
bool can_do_move(int line, int col, int next_line, int next_col);

void add_piece();
void showUI();

bool is_board_full();
bool game_over();
int score;
int high_score;
bool new_high_score;
int high_score_flag;

string to_string_(int n);

int main(int argc, char* argv[])
{
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
    if(!load_title())
    {
        cout << "Can not load title" << endl;
        return -1;
    }
    // Handle event
    SDL_Event e;
    bool quit = false;
    new_game();
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0 )
        {
            if(e.type == SDL_QUIT)
                quit = true;
            else if(e.type == SDL_KEYDOWN)
            {
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
                    quit = true;
                    break;

                    case SDLK_n:
                    new_game();
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
    g_font = TTF_OpenFont("arial.ttf", 28 );
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
    tile1->free();
    tile1 = NULL;
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
    bg_texture->render(0, 0);
//    tile1->render(200, 200);
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            tile_board[i][j]->free();
            SDL_Color color = {0, 0, 0};
            tile_board[i][j]->loadFromRenderedText(to_string_(board[i][j]), color);
            tile_board[i][j]->render(25 + 50 * i, 25 + 50 * j);
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

bool load_title()
{
    //Loading success flag
	bool success = true;

	assert(g_font != NULL);
	bg_texture = new Tile(g_renderer, g_font);
	bg_texture->loadFromFile("board.bmp");

	tile1 = new Tile(g_renderer, g_font);
	SDL_Color textColor = { 0, 0, 0 };
	string a = to_string_(4224);
	tile1->loadFromRenderedText(a, textColor);
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
	if(tile1 == NULL)
    {
        printf("Failed to load texture text!\n");
        success = false;
    }

	return success;
}

void new_game()
{
    if(new_high_score) high_score = score;
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
                        score = score + board[next_i][next_j] + board[i][j];
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
       || (board[line][col] != board[next_line][next_col] && board[next_line][next_col] != 0))
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
