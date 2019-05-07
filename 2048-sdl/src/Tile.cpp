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

void Tile::render( int x, int y )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy( renderer, texture, NULL, &renderQuad );
}

bool Tile::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( texture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

	//Return success
	return texture != NULL;
}
