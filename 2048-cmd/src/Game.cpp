#include "../include/Game.h"

Game::Game()
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            screenSurface = SDL_GetWindowSurface( window );
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );


            if(TTF_Init() == -1)
            {
                printf("SDL_ttf could not initialize! TTF Error: %s\n", TTF_GetError());
            }
            else
            {
                const char *filePath = "../../Bell.ttf";
                TTF_Font *m_font;
                m_font = TTF_OpenFont(filePath, 60);
                if(!m_font)
                {
                    printf("Failed to load font %s! SDL_ttf Error: %s\n", filePath, TTF_GetError());
                }
                SDL_Color color = { 0x77, 0x6E, 0x65, 0xFF };
                screenSurface = TTF_RenderText_Solid(m_font, "Ahihi", color);
            }

            SDL_UpdateWindowSurface( window );

            SDL_Delay( 2000 );
        }
        SDL_DestroyWindow( window );
        SDL_Quit();
    }
}

Game::~Game()
{
    //dtor
}
