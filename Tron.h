#pragma once

#include <vector>

#include "CollisionSystem.h"
#include "GameCtrlSystem.h"
#include "Manager.h"
#include "TronSystem.h"
#include "RenderSystem.h"
#include "SDLGame.h"
#include "AudioSystem.h"

class Tron {

public:
	Tron();
	virtual ~Tron();

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
	TronSystem *tronSystem_;
	CollisionSystem *collisionSystem_;
	GameCtrlSystem *gameCtrlSystem_;
	AudioSystem* audioSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
