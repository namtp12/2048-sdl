#include "../include/Tile.h"

Tile::Tile()
{
    texture = NULL;
    renderer = NULL;
    font = NULL;
    width = 0;
    height = 0;
}

Tile::Tile(SDL_Renderer *renderer)
{
    texture = NULL;
    this->renderer = renderer;
    this->font = NULL;
    width = 0;
    height = 0;
}
Tile::Tile(SDL_Renderer *renderer, TTF_Font *font)
{
    texture = NULL;
    this->renderer = renderer;
    this->font = font;
    width = 0;
    height = 0;
}

Tile::~Tile()
{
    free();
}

void Tile::free()
{
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		width = 0;
		height = 0;
	}
}

bool Tile::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	texture = newTexture;
	return texture != NULL;
}

int Tile::getWidth()
{
	return width;
}

int Tile::getHeight()
{
	return height;
}
