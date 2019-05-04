#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <cstdio>
#include "GameCmd.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Game : public GameCmd
{
public:
    Game();
    virtual ~Game();
protected:
private:
    SDL_Window* window;
    SDL_Surface* screenSurface;

};

#endif // GAME_H
