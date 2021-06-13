#pragma once

#include "System.h"
#include <SDL.h>

class GhostsSystem: public System {
public:
	GhostsSystem();
	void init() override;
	void update() override;
	virtual void receive(const msg::Message& msg) override;

private:
	void onCollisionWithPacMan(Entity *e);
	void addGhosts(std::size_t n);
	void disableAll();

	Uint32 lastTimeAdded_;
	int maxGhosts_;
	int numOfGhosts_;

};

