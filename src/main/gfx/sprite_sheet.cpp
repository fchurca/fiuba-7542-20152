#include <iostream>

#include "sprite_sheet.h"
#include "game_window.h"

SpriteSheet::SpriteSheet( std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, int fps, int delay, GameWindow & owner) : owner(owner){
	std::cerr << "Creating SpriteSheet " << this
		<< " with path " << pPath
		<< " owned by " << &owner << std::endl;
	this->path = pPath;

	texture = nullptr;
	initialized = false;

	this->pixel_ref_x = pixelRefX;
	this->pixel_ref_y = pixelRefY;
	this->alto_sprite = altoSprite;
	this->ancho_sprite = anchoSprite;
	this->total_sprites = cantSprites;
	this->fps = fps;	// segundos
	this->delay = delay;	// segundos
	this->currentFrame = 0;
	this->counter = 0;
	this->delayCounter = 0;
	this->tick = 0;
	
}

SpriteSheet::~SpriteSheet(){
	std::cerr << "Killing SpriteSheet " << this
		<< " with path " << path
		<< " owned by " << &owner << std::endl;
	clear();
}

void SpriteSheet::clear(){
	if(texture){
		SDL_DestroyTexture( texture );
		texture = nullptr;
	}
}

SDL_Texture*  SpriteSheet::getLoadedTexture( SDL_Renderer* renderer ){
	if( !initialized )
		initialized = loadTexture( renderer );
	return texture;
}

bool SpriteSheet::loadTexture( SDL_Renderer* renderer ){
	//	Libera la carga anterior, para poder recargar
	clear();

	//	Carga la imagen desde el path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

	if(!loadedSurface) {
		texture = nullptr;
		Logger::getInstance()->writeError( "No se puede cargar la imagen " + path + "! - " + IMG_GetError() );
		//	TODO: CARGAR UNA IMAGEN DEFAULT
	}
	else {
		//	Textura de la superficie
		texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );

		//	Libera la superficie
		SDL_FreeSurface( loadedSurface );
	}
	return texture;
}

void SpriteSheet::render(Entity & entity, int frame, SDL_Renderer* renderer ){

	auto x = entity.getX();
	auto y = entity.getY();
	auto direction = entity.getDirection();

	auto currentTick = SDL_GetTicks();	//Tiempo actual en milisegundos
	auto currentFrame = counter % total_sprites;	//Aca se debe usar el frame actual desde el estado de la entidad
	auto diffTime = currentTick - this->tick;	//Tiempo transcurrido entre render y render

	x -= owner.focus_x;
	y -= owner.focus_y;

	// Todas las entidades del mismo tipo tienen el mismo fps y delay. 

	if ( (this->fps == 0) || (diffTime >= (1000 / this->fps ) ) ) {

		//	Conversion isometrica 
		int screenX = ((x - y) * TILE_WIDTH_DEFAULT / 2) + (ANCHO_DEFAULT) / 2;
		int screenY = ((x + y) * TILE_HEIGHT_DEFAULT / 2) + (ALTO_DEFAULT - TILE_HEIGHT_DEFAULT) / 2;

		//	Ubicacion donde dibujar
		SDL_Rect renderQuad = { screenX - pixel_ref_x , screenY - pixel_ref_y, ancho_sprite, alto_sprite };

		//	Parte de la imagen a levantar
		SDL_Rect clip = { currentFrame * ancho_sprite, direction * alto_sprite, ancho_sprite, alto_sprite };

		//Esto es porque los frame del mago estan en sentido contrario al del chancho
		//cdo todas las imagenes esten en el mismo sentido esto vuela
		if (path.compare("resources//mago.png") == 0) {
			clip.x = direction * ancho_sprite;
			clip.y = currentFrame * alto_sprite;
		}

		//	Dibujado
		//	TODO: Verificar si renderQuad realmente se pisa con la pantalla
		SDL_RenderCopy( renderer, getLoadedTexture( renderer ), &clip, &renderQuad );

		if ( ( (currentFrame != 0) || (diffTime >= (this->delay * 1000)) ) ) {
			//	Actualizo el tick
			this->tick = currentTick;
			counter++;
		}
	}
}

void SpriteSheet::update(){
	/*if ( ( (currentFrame != 0) || (GameTimer::getDiffTime() >= (this->delay * 1000)) ) ) {
		counter++;
	}*/
}