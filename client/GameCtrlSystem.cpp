#include "GameCtrlSystem.h"

#include "SDLGame.h"
#include "Manager.h"
#include "GameState.h"
#include "TronSystem.h"
#include "messages.h"
#include "Serializable.h"
#include "SocketSystem.h"

GameCtrlSystem::GameCtrlSystem() :
	System(ecs::_sys_GameCtrl), //
	gameState_(nullptr) {
}

void GameCtrlSystem::init() {
	//Creamos la entidad que va a almacenar el estado del juego
	gameStateEntity_ = mngr_->addEntity();
	gameState_ = gameStateEntity_->addComponent<GameState>();

	//Avisamos a todo el mundo que nos encontramos en el menu
	mngr_->send<msg::Message>(msg::_ARRIVED_TO_MENU);
}

void GameCtrlSystem::update() {
	if (gameState_->state_ == GameState::RUNNING)
		return;
	
	//Si se pulsa el enter se comunica al server
	auto ih = game_->getInputHandler();
	if (ih->keyDownEvent() && ih->isKeyDown(SDLK_RETURN)){
		Key message(Key::keyType::ENTER);
 		mngr_->getSystem<SocketSystem>(ecs::SysId::_sys_Socket)->sendToServer(message);
	}
}

void GameCtrlSystem::receive(const msg::Message& msg)
{
	//Cambio de estados indicados por el servidor/socketSystem
	switch (msg.id)
	{
		case msg::_GAME_OVER: {
			gameState_->state_ = GameState::OVER;
			gameState_->winner_ = static_cast<const msg::GameOverMsg&>(msg).winner;
		 	break;
		}
		case msg::_GAME_START: {
			gameState_->state_ = GameState::RUNNING;
		 	break;
		}
		case msg::_ARRIVED_TO_MENU: {
			gameState_->state_ = GameState::READY;
		 	break;
		}
		default:
			break;
	}
}