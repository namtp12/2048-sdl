#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_Texture *g_texture = NULL;
SDL_Texture* load_texture(string path);

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
            }
        }
    }
    return success;
}

bool load_media()
{
    bool success = true;
    const char* file_path = "board.bmp";
    g_texture = load_texture(file_path);
    if(g_texture == NULL)
    {
        cout << "Can not load texture!: " << file_path << endl;
        success = false;
    }
    return success;
}

void close()
{
    SDL_DestroyTexture(g_texture);
    g_texture = NULL;
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
    SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);
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
