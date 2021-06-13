#include "GameCtrlSystem.h"

#include "FoodSystem.h"
#include "SDLGame.h"
#include "Manager.h"
#include "GameState.h"
#include "GhostsSystem.h"
#include "PacManSystem.h"
#include "messages.h"


GameCtrlSystem::GameCtrlSystem() :
		System(ecs::_sys_GameCtrl), //
		gameState_(nullptr) {
}


void GameCtrlSystem::init() {
	Entity *e = mngr_->addEntity();
	gameState_ = e->addComponent<GameState>();
	mngr_->setHandler(ecs::_hdlr_GameStateEntity, e);
	mngr_->send<msg::Message>(msg::_ARRIVED_TO_MENU);
}

void GameCtrlSystem::update() {

	if ( gameState_->state_ == GameState::RUNNING )
		return;

	auto ih = game_->getInputHandler();

	if ( ih->keyDownEvent() && ih->isKeyDown(SDLK_RETURN)) {
		switch ( gameState_->state_) {
		case GameState::READY:
			gameState_->state_ = GameState::RUNNING;

			mngr_->send<msg::StartGameMsg>(2, 10); //aviso al resto de sistemas
			break;
		case GameState::OVER:
			gameState_->state_ = GameState::READY;
			gameState_->score_ = 0;
			gameState_->won_ = false;

			mngr_->send<msg::Message>(msg::_ARRIVED_TO_MENU); //aviso al resto de sistemas
			break;
		default:
			assert(false); // should not be rechable
			break;
		}
	}
}

void GameCtrlSystem::receive(const msg::Message& msg)
{
	switch (msg.id)
	{
	case msg::_NO_MORE_FOOD: {
		onNoMoreFood();
		break;
	}
	case msg::_PAC_MAN_DEATH: {
		onPacManDeath();
		break;
	}
	default:
		break;
	}
}

void GameCtrlSystem::onPacManDeath() {
	gameState_->state_ = GameState::OVER;
	gameState_->won_ = false;
	mngr_->send<msg::GameOverMsg>(gameState_->won_);
}

void GameCtrlSystem::onNoMoreFood() {
	gameState_->state_ = GameState::OVER;
	gameState_->won_ = true;
	mngr_->send<msg::GameOverMsg>(gameState_->won_);
}