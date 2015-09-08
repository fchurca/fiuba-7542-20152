#include "sprite_sheet.h"

SpriteSheet::SpriteSheet( std::string pPath ){
	path = pPath;
	texture = NULL;
	initialized = false;
}

SpriteSheet::~SpriteSheet(){
	free();
}

void SpriteSheet::free(){
	if( texture != NULL ){
		SDL_DestroyTexture( texture );
		texture = NULL;
	}
}

SDL_Texture*  SpriteSheet::getLoadedTexture( SDL_Renderer* renderer ){
	if( !initialized )
		initialized = loadTexture( renderer );
	return texture;
}

bool SpriteSheet::loadTexture( SDL_Renderer* renderer ){
	//	Libera la carga anterior, para poder recargar
	free();

	//	Carga la imagen desde el path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

	if( loadedSurface == NULL ) {
		texture = NULL;
		Logger::getInstance()->writeError( "No se puede cargar la imagen " + path + "! - " + IMG_GetError() );
		//	TODO: CARGAR UNA IMAGEN DEFAULT
	}
	else {
		//	Color de Fondo
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//	Textura de la superficie
		texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );

		//	Libera la superficie
		SDL_FreeSurface( loadedSurface );
	}
	return texture != NULL;
}

void SpriteSheet::render( SDL_Renderer* renderer ){
	//	Ubicacion donde dibujar
	SDL_Rect renderQuad = { 100, 100, 100, 100 };

	//	Parte de la imagen a levantar
	SDL_Rect clip = { 10, 10, 100, 100 };

	//	Dibujado
	SDL_RenderCopy( renderer, getLoadedTexture( renderer ), &clip, &renderQuad );
}