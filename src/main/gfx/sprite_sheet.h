#ifndef __SPRITESHEET_H__
#define __SPRITESHEET_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "game_timer.h"
#include "../log/logger.h"
#include "../model/entity.h"
#include "../defines/defines.h"

class GameWindow;

class SpriteSheet {
private:
	std::string path;

	int fps;	//Animacion
	int delay;	//Animacion
	int counter; //esto despues vuela

	int pixel_ref_x;
	int pixel_ref_y;
	int total_sprites;
	int alto_sprite;
	int ancho_sprite;

	bool initialized;//= FALSE;
	SDL_Texture* texture;// = NULL;
	int getFrameDirection(Directions direction);
	bool loadTexture( SDL_Renderer* renderer );
	void clear();

public:
	GameWindow & owner;
	SpriteSheet( std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, int fps, int delay, GameWindow & owner );
	~SpriteSheet();
	void render(Entity& entity, int frame, SDL_Renderer* renderer );
	SDL_Texture* getLoadedTexture( SDL_Renderer* renderer );
};

#endif // __SPRITESHEET_H__
