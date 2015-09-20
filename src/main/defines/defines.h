#ifndef __DEFINES_H__
#define __DEFINES_H__

enum Directions {
	SOUTH_EAST = 0,
	SOUTH = 1,
	SOUTH_WEST = 2,
	WEST = 3,
	NORTH_WEST = 4,
	NORTH = 5,
	NORTH_EAST = 6,
	EAST = 7
};

//Imagen Default
#define IMG_UNASIGNED  "resources//unasigned.png"	// poner una imagen default

//Archivo Configuracion Default
#define ARCHIVO_CONFIGURACION_DEFAULT  "configuracionDefault.yaml"

//Archivo Configuracion
#define CONFIG_FILE_PATH "configuracion.yaml"

//Pantalla Default
#define ANCHO_DEFAULT  640
#define ALTO_DEFAULT  480

//Margen en pantalla DEFAULT
#define MARGEN_PANTALLA_DEFAULT  60

//Configuracion Default
#define VELOCIDAD_PERSONAJE_DEFAULT 0
#define MARGEN_SCROLL_DEFAULT  0
#define VELOCIDAD_SCROLL_DEFAULT  0

//Tipo Entidad Default
#define NOMBRE_TIPO_ENTIDAD_DEFAULT  "ENTIDAD"
#define IMAGEN_TIPO_ENTIDAD_DEFAULT  "IMAGEN"
#define ANCHO_BASE_DEFAULT  0
#define ALTO_BASE_DEFAULT  0
#define PIXELX_REF_DEFAULT  0
#define PIXELY_REF_DEFAULT  0
#define ALTO_SPRITE_DEFAULT  0
#define ANCHO_SPRITE_DEFAULT  0
#define CANTIDAD_SPRITES_DEFAULT  0
#define FPS_DEFAULT  0
#define DELAY_DEFAULT  0

//Escenario Default
#define NOMBRE_ESCENARIO_DEFAULT  "ESCENARIO"
#define SIZE_X_DEFAULT  0
#define SIZE_Y_DEFAULT  0

//Entidad Default
#define POS_X_DEFAULT  0
#define POS_Y_DEFAULT  0
#define TIPO_DEFAULT  "ENTIDAD"

//Log Level
#define LOG_LEVEL_OFF  0
#define LOG_LEVEL_ERROR  1
#define LOG_LEVEL_WARNING  2
#define LOG_LEVEL_INFORMATION  3

//Tile
#define TILE_WIDTH_DEFAULT  64
#define TILE_HEIGHT_DEFAULT  32
#endif
