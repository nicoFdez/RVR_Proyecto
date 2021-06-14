#include "Tron.h"
#include <assert.h>


#include "InputHandler.h"
#include "SDL_macros.h"

using namespace std;

Tron::Tron() :
		game_(nullptr), //
		mngr_(nullptr), //
		exit_(false) {
	initGame();
}

Tron::~Tron() {
	closeGame();
}

void Tron::initGame() {

	game_ = SDLGame::init("Stars", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	// Initialize the pool, for the rest of factories it is not needed,
	// we will be using DefFactory for simplicity (e.g., in addEntity,
	// addComponnet, send)
	//

	// create the manager
	mngr_ = new Manager(game_);


	// create the systems
	tronSystem_ = mngr_->addSystem<TronSystem>();
	renderSystem_ = mngr_->addSystem<RenderSystem>();
	collisionSystem_ = mngr_->addSystem<CollisionSystem>();
	gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>();
	audioSystem_ = mngr_->addSystem<AudioSystem>();
}

void Tron::closeGame() {
	delete mngr_;
}

void Tron::start() {
	exit_ = false;
	auto ih = InputHandler::instance();

	while (!exit_) {
		Uint32 startTime = game_->getTime();
		SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x000000FF));
		SDL_RenderClear(game_->getRenderer());

		ih->update();
		if (ih->keyDownEvent()) {
			if (ih->isKeyDown(SDLK_ESCAPE)) {
				exit_ = true;
				break;
			}
		}

		mngr_->refresh();

		gameCtrlSystem_->update();
		tronSystem_->update();
		collisionSystem_->update();
		renderSystem_->update();
		audioSystem_->update();

		// this is needed for sending the messages!
		mngr_->flushMessages();

		SDL_RenderPresent(game_->getRenderer());

		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

