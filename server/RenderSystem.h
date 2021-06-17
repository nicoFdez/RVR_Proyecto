#pragma once
#include "GameState.h"
#include "System.h"
#include "Texture.h"

class RenderSystem : public System {
public:
	RenderSystem();
	void update() override;

private:
	void drawAnimated(Entity* e);
	void drawImage(Texture image, SDL_Rect renderRect);
	void drawTronPlayers(GameState* gs);
	void drawMap(GameState* gs);
	void drawState(GameState* gs);
};
