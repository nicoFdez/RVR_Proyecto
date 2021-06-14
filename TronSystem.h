#pragma once

#include "System.h"
#include "Transform.h"
#include "Entity.h"

class TronSystem: public System {
public:
	TronSystem();
	void init() override;
	void update() override;
	virtual void receive(const msg::Message& msg) override;

private:
	void resetPacManPosition();
	Entity *pacman_;
	Transform *tr_;
};

