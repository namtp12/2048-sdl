#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <iostream>

using namespace std;

class Tile
{
    public:
        Tile();
        Tile(SDL_Renderer *renderer);
        Tile(SDL_Renderer *renderer, TTF_Font *font);
        virtual ~Tile();
        bool loadFromFile( std::string path );
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
		void free();
		int getWidth();
		int getHeight();
    protected:
    private:
        SDL_Texture *texture;
        SDL_Renderer *renderer;
        TTF_Font *font;
        int width;
        int height;
};

#endif // TILE_H
