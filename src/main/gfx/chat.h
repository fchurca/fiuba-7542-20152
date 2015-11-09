#ifndef __GFX_CHAT_H__
#define __GFX_CHAT_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;

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
	void draw(SDL_Renderer* renderer, std::string inputText);
	Chat(GameWindow& owner);
	~Chat();
};
#endif // __GFX_CHAT_H__