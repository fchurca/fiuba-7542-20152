#ifndef __GFX_ISOVIEW_H__
#define __GFX_ISOVIEW_H__
#include <string>
#include <SDL2/SDL.h>
#include "sprite_sheet.h"
#include "../model/geometry.h"

class GameWindow;
class RulesetParser;

class IsoView {
private:
	r2 size;
	r2 offset;
	std::map<std::string, std::shared_ptr<SpriteSheet>> spriteSheets;
public:
	GameWindow& owner;
	SDL_Point getSize();
	void draw();
	void drawRhombus(r2 corner1, r2 corner2);
	IsoView(GameWindow& owner, RulesetParser& rulesetParser);
	r2 screenToBoardPosition(SDL_Point screenPos);
	SDL_Point boardToScreenPosition(r2 boardPos);
	bool canDraw(std::shared_ptr<Entity> entity);
	void addSpriteSheet(std::string name, std::string pPath, int pixelRefX, int pixelRefY, int altoSprite, int anchoSprite, int cantSprites, double fps, double delay);
	void update();
	~IsoView();
};
#endif // __GFX_ISOVIEW_H__
