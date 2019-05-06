#ifndef TILEBOARD_H
#define TILEBOARD_H

#include <SDL2/SDL.h>

class TileBoard
{
    public:
        TileBoard();
        virtual ~TileBoard();
    protected:
    private:
        SDL_Renderer *drawer;
};

#endif // TILEBOARD_H
