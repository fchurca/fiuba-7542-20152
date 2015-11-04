#include <sstream>

#include "sprite_sheet.h"
#include "isoview.h"
#include "game_window.h"

SpriteSheet::SpriteSheet( std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay, IsoView & owner) : owner(owner){
	std::stringstream message;
	message << "Creating SpriteSheet " << this
		<< " with path " << pPath
		<< " owned by " << &owner;
	Logger::getInstance()->writeInformation(message.str());
	this->path = pPath;

	texture = nullptr;
	textureFOG = nullptr; //Creamos para prueba de fog un nuevo texture
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
	this->tick = Clock::now();
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
	if (textureFOG) {
		SDL_DestroyTexture(textureFOG);
		textureFOG = nullptr;
	}
}

SDL_Texture*  SpriteSheet::getLoadedTexture( SDL_Renderer* renderer, Visibility state, bool playerIsActive ){
	if( !initialized )
		initialized = loadTexture( renderer );

	if(state==VISIBLE && playerIsActive)
		return texture;
	return textureFOG;
}

bool SpriteSheet::loadTexture( SDL_Renderer* renderer ) {
	//	Libera la carga anterior, para poder recargar
	clear();
	//	Carga la imagen desde el path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	//	Si no se cargo la imagen, cargo default
	if(!loadedSurface) {
		texture = nullptr;
		textureFOG = nullptr;
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
	}
	//	La default siempre deberia poder cargarla
	if(!loadedSurface) {
		texture = nullptr;
		textureFOG = nullptr;
		Logger::getInstance()->writeError( "No se puede cargar la imagen Default ! " );
	} else {
		//	Textura de la superficie
		texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		textureFOG = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		SDL_SetTextureAlphaMod(textureFOG, 100);
		//	Libera la superficie
		SDL_FreeSurface( loadedSurface );
	}
	return texture && textureFOG;
}

void SpriteSheet::render(Entity & entity, SDL_Renderer* renderer){
	Visibility state = owner.owner.player.getVisibility(entity);
	bool playerIsActive = entity.owner.getActive();

	if (total_sprites == 0){
		currentFrame = 0;
		Logger::getInstance()->writeWarning(" La cantidad de sprites debe ser mayor a cero " + path);
	}else{
		currentFrame = counter % total_sprites;
		if(currentFrame == 0)
			counter = 0;
	}

	//	Ubicacion donde dibujar
	auto renderQuad = targetRect(entity);
	//	Parte de la imagen a levantar
	SDL_Rect clip = { entity.getDirection() * ancho_sprite, currentFrame * alto_sprite, ancho_sprite, alto_sprite };

	//	Dibujado
	if (state != INVISIBLE) //Aca hay que usar el canDraw
		SDL_RenderCopy(renderer, getLoadedTexture(renderer, state, playerIsActive), &clip, &renderQuad);
}

void SpriteSheet::update(){
	// Todas las entidades del mismo tipo tienen el mismo fps y delay. 
	auto currentTick = owner.owner.owner.timer.getCurrent();
	auto diffTime = currentTick - tick;
	if ( ( (currentFrame != 0) || (diffTime >= Ms((long)(delay * 1000))) ) ) {
		if ( (this->fps == 0) || (diffTime >= Ms((long)(1000 / fps) ) ) ) {
			//	Actualizo el tick del sprite sheet
			tick = currentTick;
			counter++;
		}
	}
}

SDL_Rect SpriteSheet::targetRect(Entity& entity) {
	auto screenPos = owner.boardToScreenPosition(entity.getPosition());
	SDL_Rect renderQuad = { screenPos.x - pixel_ref_x , screenPos.y - pixel_ref_y, ancho_sprite, alto_sprite };
	return renderQuad;
}
