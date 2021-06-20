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

	//Creacion de SDL
	game_ = SDLGame::init("Tron", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	// create the manager
	mngr_ = new Manager(game_);

	// creacion de los sistemas que componen el juego
	tronSystem_ = mngr_->addSystem<TronSystem>();
	renderSystem_ = mngr_->addSystem<RenderSystem>();
	gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>();
	audioSystem_ = mngr_->addSystem<AudioSystem>();
	socketSystem_ = mngr_->addSystem<SocketSystem>(s, p);
}

	//Limpiamos la ejecucion de los sistemas relacionados con el juego
void Tron::closeGame() {
	delete mngr_;
}

void Tron::start() {
	exit_ = false;
	auto ih = InputHandler::instance();

	//Me concecto al server
	Key messageLOGIN;
	messageLOGIN.connect = true; 
	socketSystem_->sendToServer(messageLOGIN);

	//Bucle ppal del juego
	while (!exit_) {
		Uint32 startTime = game_->getTime();
		SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x000000FF));
		SDL_RenderClear(game_->getRenderer());

		//Salida del bucle
		ih->update();
		//En caso de que se pulse la X de la ventana de SDL para cerrar el juego
		if (ih->getClose()) {
			exit_ = true;
			Key exit;
			//Se avisa al servidor de que el cliente se ha desconectado
			exit.key = Key::keyType::ESC;
			mngr_->getSystem<SocketSystem>(ecs::SysId::_sys_Socket)->sendToServer(exit);	

			break;
		}

		//actualizacion de entidades y de eventos del juego
		mngr_->refresh();

		//Update de los sistemas del juego
		gameCtrlSystem_->update();
		tronSystem_->update();
		renderSystem_->update();
		audioSystem_->update();
		socketSystem_->update();

		//Envio de todos los mensajes que se hayan almacenado por los sistemas
		mngr_->flushMessages();

		//Renderizado de la escena
		SDL_RenderPresent(game_->getRenderer());

		//Generamos un delay en caso de que el juego se este ejecutando con demasiada rapidez
		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}