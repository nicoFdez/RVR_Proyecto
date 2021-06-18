#pragma once

#include <vector>

#include "GameCtrlSystem.h"
#include "Manager.h"
#include "TronSystem.h"
#include "RenderSystem.h"
#include "SDLGame.h"
#include "AudioSystem.h"
#include "SocketSystem.h"

class Tron {
public:
	Tron(const char * s, const char * p);
	virtual ~Tron();

	// from SDLGame
	void start();

private:
	void initGame(const char * s, const char * p);
	void closeGame();

	SDLGame* game_;
	Manager* mngr_;
	bool exit_;

	RenderSystem* renderSystem_;
	TronSystem* tronSystem_;
	GameCtrlSystem* gameCtrlSystem_;
	AudioSystem* audioSystem_;
	SocketSystem* socketSystem_;

	const static int _WINDOW_WIDTH_ = 500;
	const static int _WINDOW_HEIGHT_ = 500;
};
