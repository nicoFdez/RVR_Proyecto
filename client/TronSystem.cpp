#include "TronSystem.h"

#include "InputHandler.h"
#include "Manager.h"
#include "SDLGame.h"
#include "AnimatedImageComponent.h"
#include "GameState.h"
#include "SDL_macros.h"
#include "GameCtrlSystem.h"
#include "SocketSystem.h"

TronSystem::TronSystem() :
	System(ecs::_sys_Tron), ///
	player1_(nullptr), //
	player2_(nullptr),
	tr1_(nullptr) {
}

void TronSystem::init() {
	movementTimer = 200;
	lastTickMoved = game_->getTime();
	tamCas = game_->getWindowWidth()/50;
	lastKeyPressed = Key::keyType::NONE;

	encasillado = vector <vector<casilla>>(50, vector<casilla>(50, casilla()));

	//Establecemos posiciones de los Rects
	for (int i = 0; i < encasillado.size(); i++) {
		for (int j = 0; j < encasillado.size(); j++) {
			encasillado[i][j].renderRect = RECT(i * tamCas, j * tamCas, tamCas, tamCas);
		}
	}

	player1_ = mngr_->addEntity();
	player2_ = mngr_->addEntity();

	tr1_ = player1_->addComponent<Transform>();
	tr2_ = player2_->addComponent<Transform>();

	//reset();
	tr1_->width_ = tr1_->height_ = 10;
	tr2_->width_ = tr2_->height_ = 10;

	auto animatedImage = player1_->addComponent<AnimatedImageComponent>();
	animatedImage->setFrameTime(100);
	Texture* spritesTex = game_->getTextureMngr()->getTexture(
		Resources::PacManSprites);
	for (int i = 0; i < 4; i++) {
		animatedImage->addFrame(spritesTex, { i * 128, 0, 128, 128 });
	}

	animatedImage = player2_->addComponent<AnimatedImageComponent>();
	animatedImage->setFrameTime(100);
	spritesTex = game_->getTextureMngr()->getTexture(
		Resources::PacManSprites);
	for (int i = 0; i < 4; i++) {
		animatedImage->addFrame(spritesTex, { i * 128, 0, 128, 128 });
	}
}

void TronSystem::update() {
	auto gameState = mngr_->getSystem<GameCtrlSystem>(ecs::SysId::_sys_GameCtrl)->getGameStateEntity()->getComponent<GameState>(ecs::GameState);
	if (gameState->state_ != GameState::RUNNING)
		return;

	inputManagement();

}

void TronSystem::inputManagement()
{
	lastKeyPressed = Key::keyType::NONE;
	auto ih = InputHandler::instance();
	if (ih->keyDownEvent()) {
		if (ih->isKeyDown(SDLK_RIGHT)) {
			lastKeyPressed = Key::keyType::RIGHT;
		}
		else if (ih->isKeyDown(SDLK_LEFT)) {
			lastKeyPressed = Key::keyType::LEFT;
		}
		else if (ih->isKeyDown(SDLK_UP)) {
			lastKeyPressed = Key::keyType::UP;
		}
		else if (ih->isKeyDown(SDLK_DOWN)) {
			lastKeyPressed = Key::keyType::DOWN;
		}
		//Mandar mensaje con tecla pulsada
		Key message(lastKeyPressed);
 		mngr_->getSystem<SocketSystem>(ecs::SysId::_sys_Socket)->sendToServer(message);	
	}
}

void TronSystem::receive(const msg::Message& msg)
{
	// switch (msg.id)
	// {
	// case msg::_GAME_START: {
	// 	reset();
	// 	break;
	// }
	// default:
	// 	break;
	// }
}

// void TronSystem::reset() {
// 	//Tama�o
// 	tr1_->width_ = tr1_->height_ = 10.0;

// 	//Posicion en la esquina superior casilla
// 	tr1_->position_ = Vector2D((game_->getWindowWidth() - tr1_->width_) / 4,
// 		(game_->getWindowHeight() - tr1_->height_) / 4);
// 	int indiceX = tr1_->position_.getX() / tamCas;
// 	int indiceY = tr1_->position_.getY() / tamCas;
// 	tr1_->position_ = Vector2D(indiceX * tamCas, indiceY * tamCas);
// 	//Vel
// 	tr1_->rotation_ = 0.0;
// 	_dirP1 = Vector2D(1, 0);

// 	//Player 2 lo mismo
// 	tr2_->width_ = tr2_->height_ = 10.0;
// 	tr2_->position_ = Vector2D((game_->getWindowWidth() - tr2_->width_) * 3 / 4,
// 		(game_->getWindowHeight() - tr2_->height_) * 3 / 4);
// 	indiceX = tr2_->position_.getX() / tamCas;
// 	indiceY = tr2_->position_.getY() / tamCas;

// 	tr2_->position_ = Vector2D(indiceX * tamCas, indiceY * tamCas);
// 	tr2_->rotation_ = 0.0;
// 	_dirP2 = Vector2D(-1, 0);

// 	lastTickMoved = game_->getTime();

// 	//Establecemos posiciones de los Rects
// 	for (int i = 0; i < encasillado.size(); i++) {
// 		for (int j = 0; j < encasillado.size(); j++) {
// 			encasillado[i][j].miEstado = estadoCasilla::none;
// 		}
// 	}
// }

void TronSystem::setEncasillado(vector<vector<int>> mapa){

	for(int i=0;i<mapa.size();i++){
		for(int j=0;j<mapa.size();j++){
			encasillado[i][j].miEstado = (estadoCasilla)mapa[i][j];
		}
	}

}

void TronSystem::setPlayerTransform(int id, Vector2D pos, float rot){
	if(id == 1){
		tr1_->position_ = (pos*tamCas);
		tr1_->rotation_ = rot;
	}
	else if(id == 2){
		tr2_->position_ = (pos*tamCas);
		tr2_->rotation_ = rot;
	}
}
