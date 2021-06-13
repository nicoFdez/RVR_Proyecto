#include "GhostsSystem.h"

#include <cmath>
#include "SDL_macros.h"
#include "Transform.h"
#include "Entity.h"
#include "GameCtrlSystem.h"
#include "GameState.h"
#include "GhostsPool.h"
#include "Manager.h"
#include "SDLGame.h"

GhostsSystem::GhostsSystem() :
		System(ecs::_sys_Ghosts), //
		lastTimeAdded_(0), //
		maxGhosts_(10), //
		numOfGhosts_(0) {
}


void GhostsSystem::init() {
}

void GhostsSystem::update() {

	auto gameState = mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState);
	if ( gameState->state_ != GameState::RUNNING)
		return;

	// add 2 ghosts every 5sec
	if (lastTimeAdded_ + 5000 < game_->getTime()) {
		lastTimeAdded_ = game_->getTime();
		addGhosts(2);
	}

	for (auto &e : mngr_->getGroupEntities(ecs::_grp_Ghost)) {

		if (!e->isActive())
			return;

		Transform *tr = e->getComponent<Transform>(ecs::Transform);

		// with probability 5% change direction to follow pacman
		RandomNumberGenerator *r = game_->getRandGen();
		if ( r->nextInt(0, 100) < 2) {
			Vector2D pmanPositon = mngr_->getHandler(ecs::_hdlr_PacManEntity)->getComponent<
					Transform>(ecs::Transform)->position_;
			 tr->velocity_ = (pmanPositon - tr->position_).normalize() * (r->nextInt(1, 10) / 20.0);

		}


		// bounce on borders
		tr->position_ = tr->position_ + tr->velocity_;
		int x = tr->position_.getX();
		int y = tr->position_.getY();

		if (x <= 0 || x + tr->width_ >= game_->getWindowWidth() || y <= 0
				|| y + tr->height_ >= game_->getWindowHeight()) {
			tr->velocity_ = tr->velocity_.rotate(180);
		}

	}
}

void GhostsSystem::receive(const msg::Message& msg)
{
	switch (msg.id)
	{
	case msg::_COLLISION_WITH_GHOST: {
		onCollisionWithPacMan(static_cast<const msg::CollisionWithGhostMsg&>(msg).ghost);
		break;
	}
	case msg::_GAME_OVER: {
		disableAll();
		break;
	}
	case msg::_GAME_START: {
		addGhosts(static_cast<const msg::StartGameMsg&>(msg).nGhosts);
		break;
	}
	default:
		break;
	}
}


void GhostsSystem::onCollisionWithPacMan(Entity *e) {
	mngr_->send<msg::Message>(msg::_PAC_MAN_DEATH);
}

void GhostsSystem::addGhosts(std::size_t n) {

	// we cannot exceed the maximum number of ghosts
	if (numOfGhosts_ >= maxGhosts_)
		return;

	RandomNumberGenerator *r = game_->getRandGen();

	Vector2D pmanPositon = mngr_->getHandler(ecs::_hdlr_PacManEntity)->getComponent<
			Transform>(ecs::Transform)->position_;

	// ghost width and height
	int width = 30;
	int height = 30;

	for (auto i(0u); i < n; i++) {

		// select corner
		int c = r->nextInt(0, 4);

		int x = 0;
		int y = 0;

		switch (c) {
		case 0:
			x = 10;
			y = 10;
			break;
		case 1:
			x = game_->getWindowWidth() - width - 10;
			y = 10;
			break;
		case 2:
			x = game_->getWindowWidth() - width - 10;
			y = game_->getWindowHeight() - height - 10;
			break;
		case 3:
			x = 10;
			y = game_->getWindowHeight() - height - 10;
			break;
		}

		Vector2D p(x, y);

		Vector2D v = (pmanPositon - p).normalize() * (r->nextInt(1, 10) / 20.0);

		// add the entity
		Entity *e = mngr_->addEntity<GhostsPool>(p, v, 30, 30);
		if (e != nullptr) {
			e->addToGroup(ecs::_grp_Ghost);
			numOfGhosts_++;
		}
	}
}

void GhostsSystem::disableAll() {
	for( auto& e : mngr_->getGroupEntities(ecs::_grp_Ghost)) {
		e->setActive(false);
	}
	numOfGhosts_ = 0;
}
