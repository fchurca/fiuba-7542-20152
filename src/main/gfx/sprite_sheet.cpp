#include <sstream>

#include "sprite_sheet.h"
#include "game_window.h"

SpriteSheet::SpriteSheet( std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay, GameWindow & owner) : owner(owner){
	std::stringstream message;
	message << "Creating SpriteSheet " << this
		<< " with path " << pPath
		<< " owned by " << &owner;
	Logger::getInstance()->writeInformation(message.str());
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
	this->tick = 0;
}

SpriteSheet::~SpriteSheet(){
	std::stringstream message;
	message << "Killing SpriteSheet " << this
		<< " with path " << path
		<< " owned by " << &owner;
	Logger::getInstance()->writeInformation(message.str());
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

bool SpriteSheet::loadTexture( SDL_Renderer* renderer ) {
	//	Libera la carga anterior, para poder recargar
	clear();
	//	Carga la imagen desde el path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	//	Si no se cargo la imagen, cargo default
	if(!loadedSurface) {
		texture = nullptr;
		Logger::getInstance()->writeError( "No se puede cargar la imagen " + path + "! - " + IMG_GetError() );
		loadedSurface = IMG_Load( ENTIDAD_DEFAULT_IMAGEN );
		this->pixel_ref_x = ENTIDAD_DEFAULT_PIXEL_REF_X;
		this->pixel_ref_y = ENTIDAD_DEFAULT_PIXEL_REF_Y;
		this->alto_sprite = ENTIDAD_DEFAULT_ALTO_SPRITE;
		this->ancho_sprite = ENTIDAD_DEFAULT_ANCHO_SPRITE;
		this->total_sprites = ENTIDAD_DEFAULT_CANTIDAD_SPRITES;
		this->fps = ENTIDAD_DEFAULT_FPS;	// segundos
		this->delay = ENTIDAD_DEFAULT_DELAY;	// segundos
		this->currentFrame = 0;
		this->counter = 0;
		this->tick = 0;
	}
	//	La default siempre deberia poder cargarla
	if(!loadedSurface) {
		texture = nullptr;
		Logger::getInstance()->writeError( "No se puede cargar la imagen Default ! " );
	} else {
		//	Textura de la superficie
		texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		//	Libera la superficie
		SDL_FreeSurface( loadedSurface );
	}
	return texture;
}

void SpriteSheet::render(Entity & entity, int frame, SDL_Renderer* renderer){
	auto direction = entity.getDirection();

	if (total_sprites == 0){
		currentFrame = 0;
		Logger::getInstance()->writeWarning(" La cantidad de sprites debe ser mayor a cero " + path);
	}else{
		currentFrame = counter % total_sprites;
		if(currentFrame == 0)
			counter = 0;
	}

	auto screenPos = owner.boardToScreenPosition(entity.getPosition());

	//	Ubicacion donde dibujar
	SDL_Rect renderQuad = { screenPos.x - pixel_ref_x , screenPos.y - pixel_ref_y, ancho_sprite, alto_sprite };

	//	Parte de la imagen a levantar
	SDL_Rect clip = { direction * ancho_sprite, currentFrame * alto_sprite, ancho_sprite, alto_sprite };

	//	Dibujado
	//	TODO: Verificar si renderQuad realmente se pisa con la pantalla
	SDL_RenderCopy( renderer, getLoadedTexture( renderer ), &clip, &renderQuad );
}

void SpriteSheet::update(){
	// Todas las entidades del mismo tipo tienen el mismo fps y delay. 
	auto currentTick = GameTimer::getCurrent();
	auto diffTime = currentTick - this->tick;
	if ( ( (currentFrame != 0) || (diffTime >= (this->delay * 1000)) ) ) {
		if ( (this->fps == 0) || (diffTime >= (1000 / this->fps ) ) ) {
			//	Actualizo el tick del sprite sheet
			this->tick = currentTick;
			counter++;
		}
	}
}
