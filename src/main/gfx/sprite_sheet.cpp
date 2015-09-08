#include "sprite_sheet.h"

SpriteSheet::SpriteSheet( std::string pPath ){
	path = pPath;
	texture = NULL;
	initialized = false;

	fps = 0;
	delay = 0;

	ancho_base = 0;
	alto_base = 0;
	pixel_ref_x = 0;
	pixel_ref_y = 0;
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

void SpriteSheet::render( int x, int y, SDL_Renderer* renderer ){
	//	TODO: EL RENDER DEBE RECIBIR LA ENTIDAD PARA OBTENER LA UBICACION

	//	Conversion isometrica - TODO: PONER LAS CONVERSIONES EN OTRA CLASE
	int screenX = ((x / 2) - ((y * TILE_WIDTH_DEFAULT) / (TILE_HEIGHT_DEFAULT * 2)));
	int screenY = (((x * TILE_HEIGHT_DEFAULT) / (TILE_WIDTH_DEFAULT * 2)) + (y / 2));

	//	Ubicacion donde dibujar
	SDL_Rect renderQuad = { screenX, screenY, TILE_WIDTH_DEFAULT, TILE_HEIGHT_DEFAULT };

	//	Parte de la imagen a levantar
	SDL_Rect clip = { 1, 1, TILE_WIDTH_DEFAULT, TILE_HEIGHT_DEFAULT };

	//	Dibujado
	SDL_RenderCopy( renderer, getLoadedTexture( renderer ), &clip, &renderQuad );
}