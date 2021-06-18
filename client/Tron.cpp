#include "Tron.h"
#include <assert.h>

#include "InputHandler.h"
#include "SDL_macros.h"

using namespace std;

Tron::Tron(const char * s, const char * p) :
	game_(nullptr), //
	mngr_(nullptr), //
	exit_(false) {
	initGame(s, p);
}

Tron::~Tron() {
	closeGame();
}

void Tron::initGame(const char * s, const char * p) {
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
	gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>();
	audioSystem_ = mngr_->addSystem<AudioSystem>();
	socketSystem_ = mngr_->addSystem<SocketSystem>(s, p);
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
				Key exit;
				exit.key = Key::keyType::ESC;
				//Mandar mensaje de terminar
				mngr_->getSystem<SocketSystem>(ecs::SysId::_sys_Socket)->sendToServer(exit);	

				break;
			}
		}

		mngr_->refresh();

		gameCtrlSystem_->update();
		tronSystem_->update();
		renderSystem_->update();
		audioSystem_->update();
		socketSystem_->update();

		// this is needed for sending the messages!
		mngr_->flushMessages();

		SDL_RenderPresent(game_->getRenderer());

		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}