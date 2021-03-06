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

enum GameModes {
	DESTROY_CENTER = 0,
	KILL_KING = 1,
	DESTROY_FLAG = 2
};

//Archivo Configuracion
#define CONFIG_FILE_PATH "configuracion.yaml"
#define CONFIG_FILE_PATH_DEFAULT  "default//configuracionDefault.yaml"
#define RULESET_CONFIG_FILE_PATH "ruleset.yaml"
#define RULESET_CONFIG_FILE_PATH_DEFAULT  "default//rulesetDefault.yaml"
#define SCENARIO_CONFIG_FILE_PATH "scenario.yaml"
#define SCENARIO_CONFIG_FILE_PATH_DEFAULT  "default//scenarioDefault.yaml"
#define GRAPHICS_CONFIG_FILE_PATH "graphics.yaml"
#define GRAPHICS_CONFIG_FILE_PATH_DEFAULT  "default//graphicsDefault.yaml"

//Pantalla Default
#define ANCHO_DEFAULT  640
#define ALTO_DEFAULT  480
#define ANCHO_MINIMAPA_DEFAULT  200
#define ALTO_HUD_DEFAULT  120
#define SIZE_TEXT_DEFAULT  14

//Configuracion Default
#define VELOCIDAD_PERSONAJE_DEFAULT 1
#define MARGEN_SCROLL_DEFAULT  45
#define VELOCIDAD_SCROLL_DEFAULT  45
#define DT_DEFAULT 50
#define FUENTE_DEFAULT "resources//PlayfairDisplay-Regular.otf"

//Escenario Default
#define ESCENARIO_DEFAULT_NOMBRE  "escenario_default"
#define ESCENARIO_DEFAULT_SIZE_X  10
#define ESCENARIO_DEFAULT_SIZE_Y  10
#define ESCENARIO_DEFAULT_MAXRESOURCES  999999

//Tipo Entidad Default
#define ENTIDAD_DEFAULT_NOMBRE  "entidad_default"
#define ENTIDAD_DEFAULT_IMAGEN  "default//undefined.png"
#define ENTIDAD_DEFAULT_POSX  1
#define ENTIDAD_DEFAULT_POSY  1
#define ENTIDAD_DEFAULT_ANCHO_BASE  0.5
#define ENTIDAD_DEFAULT_ALTO_BASE  0.5
#define ENTIDAD_DEFAULT_PIXEL_REF_X  30
#define ENTIDAD_DEFAULT_PIXEL_REF_Y  30
#define ENTIDAD_DEFAULT_ALTO_SPRITE  32
#define ENTIDAD_DEFAULT_ANCHO_SPRITE  64
#define ENTIDAD_DEFAULT_CANTIDAD_SPRITES  1
#define ENTIDAD_DEFAULT_CANTIDAD_ACTION_SPRITES  3
#define ENTIDAD_DEFAULT_FPS  0
#define ENTIDAD_DEFAULT_DELAY  0
#define ENTIDAD_DEFAULT_SIGHT_RADIUS  0
#define ENTIDAD_DEFAULT_SPEED  0
#define ENTIDAD_DEFAULT_CAPACITY  0
#define ENTIDAD_DEFAULT_BEHAVIOUR "unit"
#define ENTIDAD_DEFAULT_HEALTH 100
#define ENTIDAD_DEFAULT_ARMOUR 0
#define ENTIDAD_DEFAULT_PRODUCTS ""
#define ENTIDAD_DEFAULT_RESOURCE_NAME ""
#define ENTIDAD_DEFAULT_HIT_FORCE 1
#define ENTIDAD_DEFAULT_HIT_RADIUS 1
//Nombre de jugador inicial (terreno, etc)
#define DEFAULT_PLAYER_NAME "Gaia"

//Protagonista Default
#define PROTAGONISTA_DEFAULT_NOMBRE "protagonista_default"
#define PROTAGONISTA_DEFAULT_IMAGEN "default//mago.png"
#define PROTAGONISTA_DEFAULT_POSX 0
#define PROTAGONISTA_DEFAULT_POSY 0
#define PROTAGONISTA_DEFAULT_ANCHO_BASE  0.5
#define PROTAGONISTA_DEFAULT_ALTO_BASE  0.5
#define PROTAGONISTA_DEFAULT_PIXEL_REF_X  30
#define PROTAGONISTA_DEFAULT_PIXEL_REF_Y  30
#define PROTAGONISTA_DEFAULT_FPS  10
#define PROTAGONISTA_DEFAULT_DELAY  0
#define PROTAGONISTA_DEFAULT_ANCHO_SPRITE  60
#define PROTAGONISTA_DEFAULT_ALTO_SPRITE  60
#define PROTAGONISTA_DEFAULT_CANTIDAD_SPRITES  5
#define PROTAGONISTA_DEFAULT_HEALTH 100
#define PROTAGONISTA_DEFAULT_ARMOUR 0
#define PROTAGONISTA_DEFAULT_PRODUCTS ""
#define PROTAGONISTA_DEFAULT_RESOURCE_NAME ""
#define PROTAGONISTA_DEFAULT_HIT_FORCE 0
#define PROTAGONISTA_DEFAULT_HIT_RADIUS 0


//Terreno Default
#define TERRENO_DEFAULT_NOMBRE "terreno_default"
#define TERRENO_DEFAULT_IMAGEN "default//pasto.png"	
#define TERRENO_DEFAULT_ANCHO_BASE  1
#define TERRENO_DEFAULT_ALTO_BASE  1
#define TERRENO_DEFAULT_PIXEL_REF_X  32
#define TERRENO_DEFAULT_PIXEL_REF_Y  0
#define TERRENO_DEFAULT_FPS  0
#define TERRENO_DEFAULT_DELAY  0
#define TERRENO_DEFAULT_ANCHO_SPRITE  64
#define TERRENO_DEFAULT_ALTO_SPRITE  32
#define TERRENO_DEFAULT_CANTIDAD_SPRITES  1
#define TERRENO_DEFAULT_SIGHT_RADIUS 0
#define TERRENO_DEFAULT_SPEED 0
#define TERRENO_DEFAULT_CAPACITY 0 
#define TERRENO_DEFAULT_BEHAVIOUR "terrain"
#define TERRENO_DEFAULT_HEALTH 0
#define TERRENO_DEFAULT_ARMOUR 0 
#define TERRENO_DEFAULT_PRODUCTS ""
#define TERRENO_DEFAULT_RESOURCE_NAME ""
#define TERRENO_DEFAULT_HIT_FORCE 0
#define TERRENO_DEFAULT_HIT_RADIUS 0

//Recurso Default
#define RECURSO_DEFAULT_NOMBRE "recurso_default"
#define RECURSO_DEFAULT_IMAGEN "default//undefined.png"	
#define RECURSO_DEFAULT_ANCHO_BASE  1
#define RECURSO_DEFAULT_ALTO_BASE  1
#define RECURSO_DEFAULT_PIXEL_REF_X  32
#define RECURSO_DEFAULT_PIXEL_REF_Y  0
#define RECURSO_DEFAULT_FPS  0
#define RECURSO_DEFAULT_DELAY  0
#define RECURSO_DEFAULT_ANCHO_SPRITE  64
#define RECURSO_DEFAULT_ALTO_SPRITE  32
#define RECURSO_DEFAULT_CANTIDAD_SPRITES  1
#define RECURSO_DEFAULT_SIGHT_RADIUS 0
#define RECURSO_DEFAULT_SPEED 0
#define RECURSO_DEFAULT_CAPACITY 0 
#define RECURSO_DEFAULT_BEHAVIOUR "resource"
#define RECURSO_DEFAULT_HEALTH 0
#define RECURSO_DEFAULT_ARMOUR 0 
#define RECURSO_DEFAULT_PRODUCTS ""
#define RECURSO_DEFAULT_RESOURCE_NAME ""
#define RECURSO_DEFAULT_HIT_FORCE 0
#define RECURSO_DEFAULT_HIT_RADIUS 0

//Recurso Default
#define ESTRUCTURA_DEFAULT_NOMBRE "estructura_default"
#define ESTRUCTURA_DEFAULT_IMAGEN "default//undefined.png"	
#define ESTRUCTURA_DEFAULT_ANCHO_BASE  1
#define ESTRUCTURA_DEFAULT_ALTO_BASE  1
#define ESTRUCTURA_DEFAULT_PIXEL_REF_X  32
#define ESTRUCTURA_DEFAULT_PIXEL_REF_Y  0
#define ESTRUCTURA_DEFAULT_FPS  0
#define ESTRUCTURA_DEFAULT_DELAY  0
#define ESTRUCTURA_DEFAULT_ANCHO_SPRITE  64
#define ESTRUCTURA_DEFAULT_ALTO_SPRITE  32
#define ESTRUCTURA_DEFAULT_CANTIDAD_SPRITES  1
#define ESTRUCTURA_DEFAULT_SIGHT_RADIUS 0
#define ESTRUCTURA_DEFAULT_SPEED 0
#define ESTRUCTURA_DEFAULT_CAPACITY 0 
#define ESTRUCTURA_DEFAULT_BEHAVIOUR "building"
#define ESTRUCTURA_DEFAULT_HEALTH 1000
#define ESTRUCTURA_DEFAULT_ARMOUR 0 
#define ESTRUCTURA_DEFAULT_PRODUCTS ""
#define ESTRUCTURA_DEFAULT_RESOURCE_NAME ""
#define ESTRUCTURA_DEFAULT_HIT_FORCE 0
#define ESTRUCTURA_DEFAULT_HIT_RADIUS 0

//Product
#define PRODUCT_DEFAULT_NAME "product"
#define COSTO_DEFAULT_NAME "recurso"
#define COSTO_DEFAULT_CANTIDAD 0

//Log Level
#define LOG_LEVEL_OFF  0
#define LOG_LEVEL_ERROR  1
#define LOG_LEVEL_WARNING  2
#define LOG_LEVEL_INFORMATION  3

//Tile
#define TILE_WIDTH_DEFAULT  64
#define TILE_HEIGHT_DEFAULT  32

//Archivo deConfiguracion del server
#define SERVER_CONFIG_FILE_PATH  "server_config.yaml"
#define SERVER_CONFIG_FILE_PATH_DEFAULT  "default/server_config_default.yaml"

// Configuracion Server DEFAULT
#define SERVER_IP_DEFAULT "localhost"
#define SERVER_PORT_DEFAULT 8001
#define SERVER_MAX_CLIENTS_DEFAULT 10

//Archivo deConfiguracion del cliente
#define CLIENT_SERVER_CONFIG_FILE_PATH  "client_config.yaml"
#define CLIENT_SERVER_CONFIG_FILE_PATH_DEFAULT  "default/client_config_default.yaml"

// Configuracion Client DEFAULT
#define CLIENT_SERVER_IP_DEFAULT "localhost"
#define CLIENT_SERVER_PORT_DEFAULT 8001

//Input
#define MAX_LENGTH_MESSAGE 20
#endif
