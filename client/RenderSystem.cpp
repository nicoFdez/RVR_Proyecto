#include "RenderSystem.h"

#include <cassert>
#include "AnimatedImageComponent.h"
#include "ecs.h"
#include "Manager.h"
#include "SDL_macros.h"
#include "SDLGame.h"
#include "Transform.h"
#include "Entity.h"
#include "GameState.h"
#include "Manager.h"
#include "SDLGame.h"
#include "Texture.h"
#include "TronSystem.h"
#include "GameCtrlSystem.h"

RenderSystem::RenderSystem() :
	System(ecs::_sys_Render) {
}

void RenderSystem::drawAnimated(Entity* e) {
	
	//Toma el transform de una entidad, saca su posicion y toma su componente de Animated... para renderizarlo

	Transform* tr = e->getComponent<Transform>(ecs::Transform);
	AnimatedImageComponent* img = e->getComponent<AnimatedImageComponent>(
		ecs::AnimatedImageComponent);
	const auto& sprite = img->getSprite(game_->getTime());
	SDL_Rect dest =
		RECT(tr->position_.getX(), tr->position_.getY(), tr->width_,
			tr->height_);
	sprite.first->render(dest, tr->rotation_, sprite.second);
}

void RenderSystem::drawImage(Texture image, SDL_Rect renderRect)
{
	//Toma una textura y la renderiza en el renderRect que le llega
	image.render(renderRect);
}

void RenderSystem::update() {
	auto gameState_ = mngr_->getSystem <GameCtrlSystem>(ecs::_sys_GameCtrl)->getGameStateEntity()->getComponent<GameState>(ecs::GameState);

	drawMap(gameState_);
	drawTronPlayers(gameState_);
	drawState(gameState_);
}

void RenderSystem::drawTronPlayers(GameState* gs) {
	if (gs->state_ != GameState::RUNNING)
		return;
	//Dibujar el jugador1
	drawAnimated(mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron)->getPlayer1());
	//Dibujar el jugador2
	drawAnimated(mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron)->getPlayer2());
}

void RenderSystem::drawMap(GameState* gs) {
	if (gs->state_ != GameState::RUNNING)
		return;

	auto casillas = mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron)->getTronMap();

	//Se recorren las casillas
	for (int i = 0; i < casillas.size(); i++) {
		for (int j = 0; j < casillas[i].size(); j++) {
			Texture* texture = nullptr;
			//Si el estado de la casilla es player1 se pinta azul
			if (casillas[i][j].miEstado == TronSystem::estadoCasilla::player1)
				texture = game_->getTextureMngr()->getTexture(Resources::Blue);
			//Si el estado de la casilla es player2 se pinta rojo
			else if (casillas[i][j].miEstado == TronSystem::estadoCasilla::player2) {
				texture = game_->getTextureMngr()->getTexture(Resources::Red);
			}
			if (texture != nullptr)
				texture->render(casillas[i][j].renderRect);
		}
	}
}


void RenderSystem::drawState(GameState* gs) {

	if (gs->state_ == GameState::RUNNING)
		return;

	//Renderizamos un mensaje u otro dependiendo del estado del juego en el que nos encontremos
	int x = 0;
	int y = 0;
	switch (gs->state_) {
	case GameState::READY: { //Mensaje del menu que invita a pulsar intro para empezar la partida
		auto startNewGameMsg = game_->getTextureMngr()->getTexture(Resources::PressEnterToStartANewGame);
		x = (game_->getWindowWidth() - startNewGameMsg->getWidth()) / 2;
		y = (game_->getWindowHeight() - startNewGameMsg->getHeight()) / 2;
		startNewGameMsg->render(x, y);
	}
		break;
	case GameState::OVER: { //Se indica el ganador y se invita apulsar intro para volver al menu
		//Intro para volver al menu
		auto toContMsg = game_->getTextureMngr()->getTexture(Resources::PressEnterToContinue);
		x = (game_->getWindowWidth() - toContMsg->getWidth()) / 2;
		y = (game_->getWindowHeight() - toContMsg->getHeight()) / 2;
		toContMsg->render(x, y);

		//Ganador
		int winner = gs->winner_;
		Texture* gameOverMsg;
		if (winner == 1) {
			gameOverMsg = game_->getTextureMngr()->getTexture(Resources::Player1Wins);
		}
		else gameOverMsg = game_->getTextureMngr()->getTexture(Resources::Player2Wins);
		x = (game_->getWindowWidth() - gameOverMsg->getWidth()) / 2;
		y = (game_->getWindowHeight() - gameOverMsg->getHeight()) / 2 - 50;
		gameOverMsg->render(x, y);

		break;
	}
	default:
		assert(false);
	}
}