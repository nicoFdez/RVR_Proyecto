#pragma once

#include <cassert>
#include <utility> // for std::pair
#include <vector>

#include "Component.h"
#include "Texture.h"

#include <iostream>

struct AnimatedImageComponent: Component {
	using frame = std::pair<Texture*, SDL_Rect>;
	AnimatedImageComponent() :
			Component(ecs::AnimatedImageComponent), //
			//sprites_(sprites), //
			sprites_(), frameTime_(0), //
			currSprite_(0), //
			lastChanged_(0) //
	{
	}

	void setFrameTime(Uint32 frameTime) {
		assert(frameTime >= 0);
		frameTime_ = frameTime;
	}

	void addFrame(Texture* tex, SDL_Rect clip) {
		sprites_.push_back( { tex, clip});
	}

	void reset() {
		sprites_.clear();
		frameTime_ = 0;
	}

	const frame& getSprite(Uint32 time) {
		if (time > lastChanged_ + frameTime_) {
			lastChanged_ = time;
			currSprite_ = (currSprite_ + 1) % sprites_.size();
		}

		return sprites_[currSprite_];
	}

private:
	std::vector<frame> sprites_;
	Uint32 frameTime_;
	Uint32 currSprite_;
	Uint32 lastChanged_;
};

