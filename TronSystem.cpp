#include "TronSystem.h"

#include "InputHandler.h"
#include "Manager.h"
#include "SDLGame.h"
#include "AnimatedImageComponent.h"
#include "GameState.h"


TronSystem::TronSystem() :
		System(ecs::_sys_PacMan), ///
		player1_(nullptr), //
		player2_(nullptr),
		tr1_(nullptr) {

			//tamCas = game_->getWindowWidth()/50;
}

void TronSystem::init() {
	player1_ = mngr_->addEntity();
	//player2_ = mngr_->addEntity();

	tr1_ = player1_->addComponent<Transform>();
	//tr2_ = player2_->addComponent<Transform>();

	resetPacManPosition();

	auto animatedImage = player1_->addComponent<AnimatedImageComponent>();
	animatedImage->setFrameTime(100);
	Texture *spritesTex = game_->getTextureMngr()->getTexture(
			Resources::PacManSprites);
	for (int i = 0; i < 4; i++) {
		animatedImage->addFrame(spritesTex, { i * 128, 0, 128, 128 });
	}

	mngr_->setHandler(ecs::_hdlr_PacManEntity, player1_);
}

void TronSystem::update() {

	auto gameState = mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState);
	if ( gameState->state_ != GameState::RUNNING)
		return;

	auto ih = InputHandler::instance();
	assert(tr1_ != nullptr);
	if (ih->keyDownEvent()) {
		if (ih->isKeyDown(SDLK_RIGHT)) {
			tr1_->rotation_ =tr1_->rotation_ + 10; //90
			tr1_->velocity_ = tr1_->velocity_.rotate(10);
			
		} else if (ih->isKeyDown(SDLK_LEFT)) {
			tr1_->rotation_ = tr1_->rotation_ - 10; //270
			tr1_->velocity_ = tr1_->velocity_.rotate(-10);
		} else if (ih->isKeyDown(SDLK_UP)) {
			//tr1_->rotation_ = 0;
			auto nv = Vector2D(0, -1).rotate(tr1_->rotation_);
			tr1_->velocity_ = nv * (tr1_->velocity_.magnitude() + 0.5);
		} else if (ih->isKeyDown(SDLK_DOWN)) {
			//tr1_->rotation_ = 180;
			auto nv = Vector2D(0, -1).rotate(tr1_->rotation_);
			tr1_->velocity_ = nv
					* std::max(0.0, (tr1_->velocity_.magnitude() - 0.5));
		}
	}

	// move, but stop on borders
	Vector2D oldPositions = tr1_->position_;

	tr1_->position_ = tr1_->position_ + tr1_->velocity_;
	int x = tr1_->position_.getX();
	int y = tr1_->position_.getY();

	if (x <= 0 || x + tr1_->width_ >= game_->getWindowWidth() || y <= 0
			|| y + tr1_->height_ >= game_->getWindowHeight()) {
		tr1_->position_ = oldPositions;
		tr1_->velocity_ = Vector2D(0.0, 0.0);
	}

}

void TronSystem::receive(const msg::Message& msg)
{
	switch (msg.id)
	{
	case msg::_GAME_START: {
		resetPacManPosition();
		break;
	}
	default:
		break;
	}
}

void TronSystem::resetPacManPosition() {
	tr1_->width_ = tr1_->height_ = 30.0;
	tr1_->position_ = Vector2D( (game_->getWindowWidth()-tr1_->width_)/2,
			(game_->getWindowHeight()-tr1_->height_)/2);
	tr1_->velocity_ = Vector2D(0.0, 0.0);
	tr1_->rotation_ = 0.0;

	// tr2_->width_ = tr2_->height_ = 30.0;
	// tr2_->position_ = Vector2D( (game_->getWindowWidth()-tr2_->width_)/4,
	// 		(game_->getWindowHeight()-tr2_->height_)/4);
	// tr2_->velocity_ = Vector2D(0.0, 0.0);
	// tr2_->rotation_ = 0.0;

}
