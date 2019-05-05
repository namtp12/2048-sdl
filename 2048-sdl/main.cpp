#include <SDL2/SDL.h>
#include <iostream>
#include <cstdio>

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

SDL_Window *g_window = NULL;
SDL_Surface *g_surface = NULL;
SDL_Surface *g_board = NULL;

bool init();

bool load_media();

void close();

void draw();

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
    draw();
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
        g_window = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(g_window == NULL)
        {
            cout << "Can not create window" << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            g_surface = SDL_GetWindowSurface(g_window);
        }
    }
    return success;
}

bool load_media()
{
    bool success = true;
    const char* file_path = "board.bmp";
    g_board = SDL_LoadBMP(file_path);
    if(g_board == NULL)
    {
        cout << "File not found!: " << file_path << endl;
        success = false;
    }
    return success;
}

void close()
{
    SDL_FreeSurface(g_surface);
    g_surface = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    SDL_Quit();
}

void draw()
{
    SDL_BlitSurface(g_board, NULL, g_surface, NULL);
    SDL_UpdateWindowSurface(g_window);
    SDL_Delay(2000);
}
