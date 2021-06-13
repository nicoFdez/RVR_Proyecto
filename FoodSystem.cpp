#include "FoodSystem.h"

#include "System.h"
#include "Entity.h"
#include "FoodPool.h"
#include "GameCtrlSystem.h"
#include "GameState.h"
#include "Manager.h"
#include "SDLGame.h"
#include "SDL_macros.h"

FoodSystem::FoodSystem() :
		System(ecs::_sys_Food), //
		numOfFoodPieces_(0) {
}

void FoodSystem::init() {
}

void FoodSystem::update() {
}

void FoodSystem::receive(const msg::Message& msg)
{
	switch (msg.id)
	{
	case msg::_COLLISION_WITH_CHERRY: {
		onEat(static_cast<const msg::CollisionWithCherryMsg&>(msg).cherry);
		break;
	}
	case msg::_GAME_START: {
		addFood(static_cast<const msg::StartGameMsg&>(msg).nCherries);
		break;
	}
	case msg::_GAME_OVER: {
		disableAll();
		break;
	}
	default:
		break;
	}
}

void FoodSystem::onEat(Entity *e) {
	// update score
	auto gameState = mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState);
	gameState->score_++;

	// disbale food
	e->setActive(false);
	numOfFoodPieces_--;

	if (numOfFoodPieces_ == 0)
		mngr_->send<msg::Message>(msg::_NO_MORE_FOOD);
}

void FoodSystem::addFood(std::size_t n) {

	RandomNumberGenerator *r = game_->getRandGen();

	// ghost width and height
	int width = 30;
	int height = 30;

	for (auto i(0u); i < n; i++) {

		int x = r->nextInt(10, game_->getWindowWidth() - width - 10);
		int y = r->nextInt(10, game_->getWindowHeight() - height - 10);
		Vector2D p(x, y);
		// add the entity
		Entity *e = mngr_->addEntity<FoodPool>(p, 30, 30);
		if (e != nullptr) {
			e->addToGroup(ecs::_grp_Food);
			numOfFoodPieces_++;
		}
	}
}

void FoodSystem::disableAll() {
	for (auto &e : mngr_->getGroupEntities(ecs::_grp_Food)) {
		e->setActive(false);
	}
	numOfFoodPieces_ = 0;
}
