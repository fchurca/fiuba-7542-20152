#ifndef __GFX_CHAT_H__
#define __GFX_CHAT_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class Chat {
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
	int maxMessages;
protected:
public:
	bool typing;
	std::vector<std::string> messages;
	void draw(std::string inputText);
	Chat(GameWindow& owner, GraphicsParser& graphicsParser);
	~Chat();
};
#endif // __GFX_CHAT_H__