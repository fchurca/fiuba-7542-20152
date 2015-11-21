#ifndef __GFX_RESOURCESLIST_H__
#define __GFX_RESOURCESLIST_H__
#include <string>
#include <SDL2/SDL.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class ResourcesList {
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
protected:
public:
	void draw();
	ResourcesList(GameWindow& owner, GraphicsParser& graphicsParser);
	~ResourcesList();
};
#endif // __GFX_RESOURCESLIST_H__