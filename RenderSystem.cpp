#include "RenderSystem.h"

#include <cassert>
#include "AnimatedImageComponent.h"
#include "ecs.h"
#include "Manager.h"
#include "ImageComponent.h"
#include "SDL_macros.h"
#include "SDLGame.h"
#include "Transform.h"
#include "Entity.h"
#include "GameState.h"
#include "Manager.h"
#include "SDLGame.h"
#include "Texture.h"

RenderSystem::RenderSystem() :
		System(ecs::_sys_Render) {
}

void RenderSystem::drawAnimated(Entity *e) {
	Transform *tr = e->getComponent<Transform>(ecs::Transform);
	AnimatedImageComponent *img = e->getComponent<AnimatedImageComponent>(
			ecs::AnimatedImageComponent);
	const auto &sprite = img->getSprite(game_->getTime());
	SDL_Rect dest =
	RECT(tr->position_.getX(), tr->position_.getY(), tr->width_,
			tr->height_);
	sprite.first->render(dest, tr->rotation_, sprite.second);
}

void RenderSystem::update() {

	auto gameState_ =
			mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(
					ecs::GameState);

	drawFood(gameState_);
	drawGhosts(gameState_);
	drawPacMan(gameState_);
	drawState(gameState_);

}

void RenderSystem::drawFood(GameState *gs) {
	if (gs->state_ != GameState::RUNNING)
		return;

	for (auto &e : mngr_->getGroupEntities(ecs::_grp_Food)) {
		drawAnimated(e);
	}
}

void RenderSystem::drawGhosts(GameState *gs) {
	if (gs->state_ != GameState::RUNNING)
		return;

	for (auto &e : mngr_->getGroupEntities(ecs::_grp_Ghost)) {
		drawAnimated(e);
	}
}

void RenderSystem::drawPacMan(GameState *gs) {
	if (gs->state_ != GameState::RUNNING)
		return;

	drawAnimated(mngr_->getHandler(ecs::_hdlr_PacManEntity));
}

void RenderSystem::drawState(GameState *gs) {

	// score
	Texture scoreMsg(game_->getRenderer(), std::to_string(gs->score_),
			game_->getFontMngr()->getFont(Resources::ARIAL24),
			{ COLOR(0xff0000ff) });
	scoreMsg.render(game_->getWindowWidth() / 2 - scoreMsg.getWidth() / 2, 10);

	if (gs->state_ == GameState::RUNNING)
		return;

	int x = 0;
	int y = 0;
	switch (gs->state_) {
	case GameState::READY: {
		auto startNewGameMsg = game_->getTextureMngr()->getTexture(Resources::PressEnterToStartANewGame);
		x = (game_->getWindowWidth() - startNewGameMsg->getWidth())/ 2;
		y = (game_->getWindowHeight() - startNewGameMsg->getHeight()) / 2;
		startNewGameMsg->render(x, y);
	}
		break;
	case GameState::OVER: {
		auto toContMsg = game_->getTextureMngr()->getTexture(Resources::PressEnterToContinue);
		x = (game_->getWindowWidth() - toContMsg->getWidth())/ 2;
		y = (game_->getWindowHeight() - toContMsg->getHeight()) / 2;
		toContMsg->render(x, y);

		auto gameOverMsg = game_->getTextureMngr()->getTexture(Resources::GameOver);
		x = (game_->getWindowWidth() - gameOverMsg->getWidth())/ 2;
		y = (game_->getWindowHeight() - gameOverMsg->getHeight()) / 2 - 50;
		gameOverMsg->render(x, y);


		break;
	}
	default:
		assert(false); // should be unreachable;

	}
}
