#pragma once

#include "System.h"

struct GameState;

class GameCtrlSystem : public System {
public:

	GameCtrlSystem();

	void init() override;
	void update() override;
	virtual void receive(const msg::Message& msg) override;
	Entity* getGameStateEntity() { return gameStateEntity_; }

private:
	void onPacManDeath();
	void onNoMoreFood();
	Entity* gameStateEntity_;
	GameState* gameState_;
};
