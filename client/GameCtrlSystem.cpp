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
	gameStateEntity_ = mngr_->addEntity();
	gameState_ = gameStateEntity_->addComponent<GameState>();
	mngr_->send<msg::Message>(msg::_ARRIVED_TO_MENU);
}

void GameCtrlSystem::update() {
	if (gameState_->state_ == GameState::RUNNING)
		return;

	auto ih = game_->getInputHandler();

	if (ih->keyDownEvent() && ih->isKeyDown(SDLK_RETURN)) {
		switch (gameState_->state_) {
		case GameState::READY:{
			//Mandar mensaje con tecla pulsada
			Key message(Key::keyType::ENTER);
			cout<<"estamos ready para jugar \n";
 			//mngr_->getSystem<SocketSystem>(ecs::SysId::_sys_Socket)->sendToServer(message);	
			// gameState_->state_ = GameState::RUNNING;
			// mngr_->send<msg::StartGameMsg>(); //aviso al resto de sistemas
		}
			break;
		case GameState::OVER:{
			// gameState_->state_ = GameState::READY;
			// mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron)->reset();
			// mngr_->send<msg::Message>(msg::_ARRIVED_TO_MENU); //aviso al resto de sistemas
		}
			break;
		default:{
			assert(false); // should not be rechable
			break;
		}
		}
	}
}

void GameCtrlSystem::receive(const msg::Message& msg)
{
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
	default:
		break;
	}
}