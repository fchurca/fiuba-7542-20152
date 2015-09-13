#include "sprite_sheet.h"

SpriteSheet::SpriteSheet( std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, int fps, int delay ){
	this->path = pPath;

	texture = NULL;
	initialized = false;

	this->pixel_ref_x = pixelRefX;
	this->pixel_ref_y = pixelRefY;
	this->alto_sprite = altoSprite;
	this->ancho_sprite = anchoSprite;
	this->total_sprites = cantSprites;
	this->fps = fps;
	this->delay = delay;
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
		//	Color de Fondo Transparente
		SDL_SetColorKey(loadedSurface, SDL_TRUE | SDL_RLEACCEL, SDL_MapRGB( loadedSurface->format, 255, 0, 255));

		//	Textura de la superficie
		texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );

		//	Libera la superficie
		SDL_FreeSurface( loadedSurface );
	}
	return texture != NULL;
}

void SpriteSheet::render( int x, int y, int frame, Directions direction, SDL_Renderer* renderer ){

	//	Conversion isometrica - TODO: PONER LAS CONVERSIONES EN OTRA CLASE
	int screenX = ((x / 2) - ((y * TILE_WIDTH_DEFAULT) / (TILE_HEIGHT_DEFAULT * 2)));
	int screenY = (((x * TILE_HEIGHT_DEFAULT) / (TILE_WIDTH_DEFAULT * 2)) + (y / 2));

	//	Ubicacion donde dibujar
	SDL_Rect renderQuad = { screenX - pixel_ref_x + ANCHO_DEFAULT/2, screenY - pixel_ref_y, ancho_sprite, alto_sprite };

	int image = fps % total_sprites;	//aca se debe usar el frame actual

	//	Parte de la imagen a levantar
	SDL_Rect clip = { image * ancho_sprite, direction * alto_sprite, ancho_sprite, alto_sprite };
	//SDL_Rect clip = { direction *ancho_sprite, image *  alto_sprite, ancho_sprite, alto_sprite };

	//	Dibujado
	SDL_RenderCopy( renderer, getLoadedTexture( renderer ), &clip, &renderQuad );
	fps++;
}
