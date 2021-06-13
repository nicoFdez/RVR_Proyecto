#pragma once
#include "GameState.h"
#include "System.h"



class RenderSystem: public System {
public:
	RenderSystem();
	void update() override;

private:
	void drawAnimated(Entity *e);
	void drawFood(GameState* gs);
	void drawGhosts(GameState* gs);
	void drawPacMan(GameState* gs);
	void drawState(GameState* gs);
};

