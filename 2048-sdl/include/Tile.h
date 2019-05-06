#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <iostream>

using namespace std;

class Tile
{
    public:
        Tile();
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
        int width;
        int height;
};

#endif // TILE_H
