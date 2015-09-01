#ifndef __GAME_H__
#define __GAME_H__

class Game {
private:
	bool endOfGame();	
	void processInput();
    void update();
    void render();
public:
	Game();
	int start();
	~Game();
};
#endif //__GAME_H__