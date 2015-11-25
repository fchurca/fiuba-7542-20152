#include <algorithm>
#define NOMINMAX // Para que nadie nos redefina min max
#include <sstream>

#include "game_window.h"

#include "../parser_yaml/graphics_parser.h"
#include "../parser_yaml/ruleset_parser.h"

using namespace std;

bool GameWindow::sdlInitialized = false;

bool GameWindow::initialize() {
	Logger::getInstance()->writeInformation("Initializing graphics");
	if (GameWindow::sdlInitialized) {
		Logger::getInstance()->writeWarning("SDL already initialized");
	} else {
		atexit(SDL_Quit);
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 || TTF_Init() == -1) {
			Logger::getInstance()->writeError("SDL could not initialize!");
			Logger::getInstance()->writeError(SDL_GetError());
			GameWindow::sdlInitialized = false;
		} else {
			GameWindow::sdlInitialized = true;
		}
	}
	return GameWindow::sdlInitialized;
}

GameWindow::GameWindow(Game& owner, Player& player, GraphicsParser& graphicsParser, RulesetParser& rulesetParser) :
	AClient(owner, player),
	board(player.board),
	ancho_pantalla(graphicsParser.getPantalla().ancho), alto_pantalla(graphicsParser.getPantalla().alto),
	margen_pantalla(graphicsParser.getPantalla().margen_scroll), scroll_speed(graphicsParser.getPantalla().velocidad_scroll)
{
	GameWindow::initialize(); 
	window = SDL_CreateWindow(("Trabajo Práctico 7542 - " + owner.getBoard()->name + " - " + player.name).c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		ancho_pantalla, alto_pantalla,
		SDL_WINDOW_SHOWN);

	Logger::getInstance()->writeInformation("Creating renderer");
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Color: negro opaco
	SDL_RenderClear(renderer); // Limpio pantalla inicialmente
	SDL_RenderPresent( renderer );

	auto tp = graphicsParser.getPantalla();
	font = TTF_OpenFont(FUENTE_DEFAULT, graphicsParser.getPantalla().size_text);
	if (!font) {
		Logger::getInstance()->writeError("Error al abrir TTF");
	}
	inputText = "";
	minimap = std::make_shared<MiniMap>(*this, graphicsParser);
	isoview = std::make_shared<IsoView>(*this, rulesetParser);
	menu = std::make_shared<Menu>(*this, graphicsParser);
	playersList = std::make_shared<PlayersList>(*this, graphicsParser);
	chat = std::make_shared<Chat>(*this, graphicsParser);
	resourcesList = std::make_shared<ResourcesList>(*this, graphicsParser);
	commandMenu = std::make_shared<CommandMenu>(*this, graphicsParser);
	selectionMenu = std::make_shared<SelectionMenu>(*this, graphicsParser);
	sController = std::make_shared<SelectionController>(*this);
	if (player.entities().size() > 0)
		sController->setSelection(player.entities().at(0));
	focus();
	sweeping = false;
}

GameWindow::~GameWindow() {
	Logger::getInstance()->writeInformation("Destroying renderer");
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	Logger::getInstance()->writeInformation("Destroying window");
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	} else {
		Logger::getInstance()->writeWarning("Window never initialized");
	}
	TTF_CloseFont(font);
}

SDL_Renderer* GameWindow::getRenderer() {
	return renderer;
}

void GameWindow::render() {
	isoview->draw();
	//menu->draw();TODO: MENU DEBERIA CONTENER A COMMANDMENU SELECTIONMENU MINIMAP
	commandMenu->draw();
	selectionMenu->draw();
	minimap->draw();
	chat->draw(inputText);
	playersList->draw();
	resourcesList->draw();
	if (isSweeping()) {
		r2 boardClick = isoview->screenToBoardPosition(mouseDown);
		r2 boardMouse = isoview->screenToBoardPosition(mouse);
		isoview->drawRhombus(boardClick, boardMouse);
	}

	SDL_RenderPresent(renderer);
	return;
}

void GameWindow::update(){
	sController->update();
	isoview->update();
	processInput();
	render();
	return;
}

void GameWindow::processInput(){
	SDL_GetMouseState(&mouse.x, &mouse.y);
	boardMouse = isoview->screenToBoardPosition(mouse);
	scroll();
	//	Procesar input del usuario
	while(SDL_PollEvent(EventHandler::getInstance()->getEvent())) {
		auto & e = *(EventHandler::getInstance()->getEvent());
		switch(e.type) {
			case SDL_QUIT:
				owner.exit();
				break;
			case SDL_TEXTINPUT:
				if(inputText.size() < MAX_LENGTH_MESSAGE && chat->typing) //Max largo del mensaje a ingresar.
					inputText += e.text.text;
				break;
			case SDL_KEYDOWN:
				Logger::getInstance()->writeInformation("Teclado");
				switch(e.key.keysym.sym) {
					case SDLK_c:
						if(!chat->typing && commandMenu->isVisibleWorker)
							commandMenu->showOptions = true;
						break;
					case SDLK_p:
						if(!chat->typing && commandMenu->isVisibleProducer)
							commandMenu->showOptions = true;
						break;
					case SDLK_ESCAPE:
						commandMenu->showOptions = false;
						break;
					case SDLK_1:
						if (!chat->typing && commandMenu->showOptions) {
							if (commandMenu->isVisibleProducer) {
								//SELECCION DEBE PRODUCIR PRODUCTO 1.
							}
							if (commandMenu->isVisibleWorker) {
								//CONSTRUIR PRODUCTO DEL WORKER SELECCIONADO 1.
							}
						}
						break;
					case SDLK_r:
						if(!chat->typing)
							owner.restart();
						break;
					case SDLK_s:
						for (auto e : sController->getSelection()) {
							if (!chat->typing && e->owner.name == player.name) {
								board.pushCommand(make_shared<StopCommand>(e->getId()));
							}
						}
						break;
					case SDLK_F2:
						chat->typing = !chat->typing;
						inputText = "";
						break;
					case SDLK_SPACE:
						if(!chat->typing)
							focus();
						break;
					case SDLK_BACKSPACE: 
						if (chat->typing && inputText.length() > 0){
							inputText.pop_back();
						}
						break;
					case SDLK_RETURN:
						if (chat->typing) {
							chat->messages.push_back(inputText);
							inputText = "";
						}
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_LEFT) {
					SDL_GetMouseState(&mouseDown.x, &mouseDown.y);
					sweeping = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				ostringstream oss;
				oss << "Mouse en " << mouse.x << "," << mouse.y;
				// Conversion de coordenadas en pantalla a coordenadas mapa
				oss << "; mapa: " << boardMouse.x << "," << boardMouse.y;

				Logger::getInstance()->writeInformation(oss.str().c_str());
				if (EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_LEFT) {
						setSelection();
						sweeping = false;
					}
				if( EventHandler::getInstance()->getEvent()->button.button == SDL_BUTTON_RIGHT) {
					Logger::getInstance()->writeInformation("Boton derecho");
					std::shared_ptr<Entity> obj = board.findEntity(rectangle(boardMouse,r2(0,0)));
					for (auto e : sController->getSelection()) {
						if (e->owner.name == player.name) {
							if (!(SDL_GetModState()&KMOD_SHIFT)) {
								board.pushCommand(make_shared<StopCommand>(e->getId()));
							}
							board.pushCommand(make_shared<MoveCommand>(e->getId(), boardMouse));
						}
					}
					
				}
				break;
		}
	}
}

void GameWindow::scroll(){
	double ds = (double)scroll_speed * (double)(board.dt) / 1000.0; //deltascroll
	r2 df;

	if(mouse.x <= margen_pantalla) {
		auto dsi = interpolate(mouse.x, 0, margen_pantalla, ds, 0);
		df += {-dsi, dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia la izquierda");
	}
	if(mouse.x >= ancho_pantalla - margen_pantalla){
		auto dsi = interpolate(mouse.x, ancho_pantalla - margen_pantalla, ancho_pantalla, 0, ds);
		df += {dsi, -dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia la derecha");
	}
	if(mouse.y <= margen_pantalla) {
		auto dsi = interpolate(mouse.y, 0, margen_pantalla, ds, 0);
		df += {-dsi, -dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia arriba");
	}
	if(mouse.y >= alto_pantalla - margen_pantalla) {
		auto dsi = interpolate(mouse.y, alto_pantalla - margen_pantalla, alto_pantalla, 0, ds);
		df += {dsi, dsi};
		Logger::getInstance()->writeInformation("Scrolleando hacia abajo");
	}
	focus(focusPosition + df);
}

void GameWindow::focus(r2 newFocus) {
	focusPosition.x = clip(newFocus.x, 0, board.sizeX - 1);
	focusPosition.y = clip(newFocus.y, 0, board.sizeY - 1);
}

void GameWindow::focus() {
	if (sController->getSelection().size() > 0) {
		focus(sController->getSelection().at(0)->getPosition());
	}
}

r2 GameWindow::getFocus() {
	return focusPosition;
}

void GameWindow::setSelection() {
	r2 sweepStart = isoview->screenToBoardPosition(mouseDown);
	r2 sweepEnd = isoview->screenToBoardPosition(mouse);
	sController->setSelection(rectangle(sweepStart, sweepEnd - sweepStart));
}

std::string GameWindow::completeLine(std::string line, double width) {
	int txtAncho, txtAlto, espAncho, espAlto, esp;
	std::string result = line;
	TTF_SizeText(font, " ", &espAncho, &espAlto);
	TTF_SizeText(font, result.c_str(), &txtAncho, &txtAlto);
	esp = (int)floor((width - txtAncho) / espAncho);
	if (txtAncho < width) {
		if (esp * espAncho + txtAncho < width)
			esp++;
		if (esp > 0)result.insert(result.size(), esp, ' ');
	}
	return result;
}

SDL_Color GameWindow::getColor(int id) {
	Uint8 r = (id & 2) * 255;
	Uint8 g = (id & 1) * 255;
	Uint8 b = (id & 4) * 255;
	return{ r, g, b };
}

bool GameWindow::isSweeping() {
	return (sweeping && (mouse.x != mouseDown.x || mouse.y != mouseDown.y));
}

