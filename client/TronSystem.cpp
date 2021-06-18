#include "TronSystem.h"

#include "InputHandler.h"
#include "Manager.h"
#include "SDLGame.h"
#include "AnimatedImageComponent.h"
#include "GameState.h"
#include "SDL_macros.h"
#include "GameCtrlSystem.h"

TronSystem::TronSystem() :
	System(ecs::_sys_Tron), ///
	player1_(nullptr), //
	player2_(nullptr),
	tr1_(nullptr) {
}

void TronSystem::init() {
	movementTimer = 200;
	lastTickMoved = game_->getTime();
	tamCas = 10;
	lastKeyPressed = Key::keyType::NONE;

	encasillado = vector <vector<casilla>>(game_->getWindowWidth() / tamCas, vector<casilla>(game_->getWindowHeight() / tamCas, casilla()));

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

	reset();

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

	if (game_->getTime() - lastTickMoved > movementTimer) {
		lastTickMoved = game_->getTime();

		//Update el player 1
		Vector2D finalPosPlayer = updatePlayerPos(tr1_, _dirP1);
		if (checkCollision(finalPosPlayer)) {
			mngr_->send<msg::GameOverMsg>(2);
		}
		else
			encasillado[finalPosPlayer.getX()][finalPosPlayer.getY()].miEstado = estadoCasilla::player1;

		//Update el player 2
		finalPosPlayer = updatePlayerPos(tr2_, _dirP2);
		if (checkCollision(finalPosPlayer)) {
			mngr_->send<msg::GameOverMsg>(1);
		}
		else
			encasillado[finalPosPlayer.getX()][finalPosPlayer.getY()].miEstado = estadoCasilla::player2;
	}
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
	}
	//Mandar mensaje con tecla pulsada
}

Vector2D TronSystem::updatePlayerPos(Transform* trPlayer, Vector2D dirPlayer)
{
	// Movimiento del player 1
	Vector2D oldPositions = trPlayer->position_;
	trPlayer->position_ = trPlayer->position_ + (dirPlayer * tamCas);
	int x = trPlayer->position_.getX();
	int y = trPlayer->position_.getY();
	if (x < 0 || x + trPlayer->width_ > game_->getWindowWidth() || y < 0
		|| y + trPlayer->height_ > game_->getWindowHeight()) {
		trPlayer->position_ = oldPositions;
		trPlayer->velocity_ = Vector2D(0.0, 0.0);
	}
	//MarcarCasilla del player 1
	int indiceX = trPlayer->position_.getX() / tamCas;
	int indiceY = trPlayer->position_.getY() / tamCas;

	return Vector2D(indiceX, indiceY);
}

bool TronSystem::checkCollision(Vector2D pos)
{
	return encasillado[pos.getX()][pos.getY()].miEstado != estadoCasilla::none;
}

void TronSystem::receive(const msg::Message& msg)
{
	switch (msg.id)
	{
	case msg::_GAME_START: {
		reset();
		break;
	}
	default:
		break;
	}
}

void TronSystem::reset() {
	//Tama�o
	tr1_->width_ = tr1_->height_ = 10.0;

	//Posicion en la esquina superior casilla
	tr1_->position_ = Vector2D((game_->getWindowWidth() - tr1_->width_) / 4,
		(game_->getWindowHeight() - tr1_->height_) / 4);
	int indiceX = tr1_->position_.getX() / tamCas;
	int indiceY = tr1_->position_.getY() / tamCas;
	tr1_->position_ = Vector2D(indiceX * tamCas, indiceY * tamCas);
	//Vel
	tr1_->velocity_ = Vector2D(0.0, 0.0);
	tr1_->rotation_ = 0.0;
	_dirP1 = Vector2D(1, 0);

	//Player 2 lo mismo
	tr2_->width_ = tr2_->height_ = 10.0;
	tr2_->position_ = Vector2D((game_->getWindowWidth() - tr2_->width_) * 3 / 4,
		(game_->getWindowHeight() - tr2_->height_) * 3 / 4);
	indiceX = tr2_->position_.getX() / tamCas;
	indiceY = tr2_->position_.getY() / tamCas;

	tr2_->position_ = Vector2D(indiceX * tamCas, indiceY * tamCas);
	tr2_->velocity_ = Vector2D(0.0, 0.0);
	tr2_->rotation_ = 0.0;
	_dirP2 = Vector2D(-1, 0);

	lastTickMoved = game_->getTime();

	//Establecemos posiciones de los Rects
	for (int i = 0; i < encasillado.size(); i++) {
		for (int j = 0; j < encasillado.size(); j++) {
			encasillado[i][j].miEstado = estadoCasilla::none;
		}
	}
}