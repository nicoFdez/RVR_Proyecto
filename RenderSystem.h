#pragma once
#include "GameState.h"
#include "System.h"



class RenderSystem: public System {
public:
	RenderSystem();
	void update() override;

private:
	void drawAnimated(Entity *e);
	void drawTronPlayers(GameState* gs);
	void drawMap(GameState* gs);
	void drawState(GameState* gs);
};

