#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "include/Tile.h"

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

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
                    system("cls");
                    cout << "w" << endl;
                    break;

                    case SDLK_s:
                    system("cls");
                    cout << "s" << endl;
                    break;

                    case SDLK_a:
                    system("cls");
                    cout << "a" << endl;
                    break;

                    case SDLK_d:
                    system("cls");
                    cout << "d" << endl;
                    break;

                    case SDLK_q:
                    system("cls");
                    quit = true;
                    cout << "q" << endl;
                    break;

                    default:
                    //
                    break;
                }
            }
        }
        draw();
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
    TTF_CloseFont( g_font );
    g_font = NULL;
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    g_renderer = NULL;
    IMG_Quit();
    SDL_Quit();
}

void draw()
{
    SDL_RenderClear(g_renderer);
    bg_texture->render(0, 0);
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
	tile1->loadFromRenderedText("2", textColor);
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
