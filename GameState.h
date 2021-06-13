#pragma once

#include <cstdint>
#include "Component.h"

struct GameState : Component {
	enum State : uint8_t { READY, RUNNING, OVER };

	GameState() :
		Component(ecs::GameState), //
		state_(READY), //
		won_(false),
		score_(0)//
	{
	}

	State state_;
	bool won_; // true means packman won 0 packman, 1
	int score_;
};

