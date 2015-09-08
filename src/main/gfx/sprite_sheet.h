#ifndef __SPRITESHEET_H__
#define __SPRITESHEET_H__

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "../log/logger.h"

class SpriteSheet {
private:
	std::string path;
	bool initialized;//= FALSE;
	SDL_Texture* texture;// = NULL;
	bool loadTexture( SDL_Renderer* renderer );
	void free();
public:
	SpriteSheet( std::string pPath );
	~SpriteSheet();
	void render( SDL_Renderer* renderer );
	SDL_Texture* getLoadedTexture( SDL_Renderer* renderer );
};
#endif // __SPRITESHEET_H__