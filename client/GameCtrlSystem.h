#pragma once

#include "System.h"

struct GameState;

/*
* Sistema que controla el estado del juego y sus transiciones
*/
class GameCtrlSystem : public System {
public:

	GameCtrlSystem();

	void init() override;
	void update() override;
	virtual void receive(const msg::Message& msg) override;

	//Devuelve el struct del estado del juego
	Entity* getGameStateEntity() { return gameStateEntity_; }

private:
	Entity* gameStateEntity_;
	GameState* gameState_;
};
