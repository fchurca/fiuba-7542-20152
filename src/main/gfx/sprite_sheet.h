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

class IsoView;

class SpriteSheet {
private:
	std::string path;

	double fps;	
	double delay;
	int counter; 
	TimePoint tick;
	int currentFrame;
	int pixel_ref_x;
	int pixel_ref_y;
	int total_sprites;
	int alto_sprite;
	int ancho_sprite;

	bool initialized;//= FALSE;
	SDL_Texture* texture;// = NULL;
	SDL_Texture* textureFOG;// = NULL; //Creamos para prueba de fog un nuevo texture
	int getFrameDirection(Directions direction);
	bool loadTexture(SDL_Renderer* renderer);
	void clear();

public:
	IsoView & owner;
	SpriteSheet( std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay, IsoView & owner );
	~SpriteSheet();
	void render(Entity& entity, SDL_Renderer* renderer);
	void update();
	SDL_Texture* getLoadedTexture(SDL_Renderer* renderer, Visibility state, bool playerIsActive);
	SDL_Rect targetRect(Entity& entity);
};

#endif // __SPRITESHEET_H__
