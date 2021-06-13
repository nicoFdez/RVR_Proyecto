#pragma once

#include <vector>

#include "CollisionSystem.h"
#include "FoodSystem.h"
#include "GameCtrlSystem.h"
#include "GhostsSystem.h"
#include "Manager.h"
#include "PacManSystem.h"
#include "RenderSystem.h"
#include "SDLGame.h"
#include "AudioSystem.h"

class PacMan {

public:
	PacMan();
	virtual ~PacMan();

	// from SDLGame
	void start();
	void stop();

private:
	void initGame();
	void closeGame();
	void initFactories();

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;

	RenderSystem *renderSystem_;
	GhostsSystem *ghostsSystem_;
	PacManSystem *pacmanSystem_;
	FoodSystem   *foodSystem_;
	CollisionSystem *collisionSystem_;
	GameCtrlSystem *gameCtrlSystem_;
	AudioSystem* audioSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
