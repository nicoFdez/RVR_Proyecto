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
#include "TronSystem.h"
#include "GameCtrlSystem.h"

RenderSystem::RenderSystem() :
	System(ecs::_sys_Render) {
}

void RenderSystem::drawAnimated(Entity* e) {
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
	//mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron);

	drawAnimated(mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron)->getPlayer1());
	drawAnimated(mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron)->getPlayer2());
}

void RenderSystem::drawMap(GameState* gs) {
	if (gs->state_ != GameState::RUNNING)
		return;

	auto casillas = mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron)->getTronMap();

	for (int i = 0; i < casillas.size(); i++) {
		for (int j = 0; j < casillas[i].size(); j++) {
			Texture* texture = nullptr;
			if (casillas[i][j].miEstado == TronSystem::estadoCasilla::player1)
				texture = game_->getTextureMngr()->getTexture(Resources::Blue);
			else if (casillas[i][j].miEstado == TronSystem::estadoCasilla::player2) {
				texture = game_->getTextureMngr()->getTexture(Resources::Red);
			}
			if (texture != nullptr)
				texture->render(casillas[i][j].renderRect);
		}
	}
}

void RenderSystem::drawState(GameState* gs) {
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
		x = (game_->getWindowWidth() - startNewGameMsg->getWidth()) / 2;
		y = (game_->getWindowHeight() - startNewGameMsg->getHeight()) / 2;
		startNewGameMsg->render(x, y);
	}
						 break;
	case GameState::OVER: {
		auto toContMsg = game_->getTextureMngr()->getTexture(Resources::PressEnterToContinue);
		x = (game_->getWindowWidth() - toContMsg->getWidth()) / 2;
		y = (game_->getWindowHeight() - toContMsg->getHeight()) / 2;
		toContMsg->render(x, y);

		auto gameOverMsg = game_->getTextureMngr()->getTexture(Resources::GameOver);
		x = (game_->getWindowWidth() - gameOverMsg->getWidth()) / 2;
		y = (game_->getWindowHeight() - gameOverMsg->getHeight()) / 2 - 50;
		gameOverMsg->render(x, y);

		break;
	}
	default:
		assert(false); // should be unreachable;
	}
}