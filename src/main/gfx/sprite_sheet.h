#ifndef __SPRITESHEET_H__
#define __SPRITESHEET_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "../log/logger.h"
#include "../entidades/entity.h"
#include "../defines/defines.h"

class SpriteSheet {
private:
	std::string path;

	unsigned int fps;	//Animacion
	unsigned int delay;	//Animacion

	unsigned int pixel_ref_x;
	unsigned int pixel_ref_y;

	bool initialized;//= FALSE;
	SDL_Texture* texture;// = NULL;
	bool loadTexture( SDL_Renderer* renderer );
	void free();

public:
	SpriteSheet( std::string pPath );
	~SpriteSheet();
	void render( int x, int y, SDL_Renderer* renderer );
	SDL_Texture* getLoadedTexture( SDL_Renderer* renderer );
};

#endif // __SPRITESHEET_H__
