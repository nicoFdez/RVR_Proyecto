#pragma once

#include <cstdint>
#include "Component.h"


/*
* Struct que representa el estado del juego
*/
struct GameState : Component {
	enum State : uint8_t { READY, RUNNING, OVER };

	GameState() :
		Component(ecs::GameState), //
		state_(READY), //
		winner_(0)
	{
	}

	State state_;
	int winner_;
};

