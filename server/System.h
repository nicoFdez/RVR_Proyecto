#pragma once

class SDLGame;
class Manager;
#include "ecs.h"
#include "messages.h"

class System {
public:
	System(ecs::SysIdType id) :
			 id_(id), mngr_(nullptr), game_(nullptr) {
	}

	virtual ~System() {
	}

	ecs::SysIdType getId() {
		return id_;
	}

	void setGame(SDLGame* game) {
		game_ = game;
	}

	void setMngr(Manager* mngr) {
		mngr_ = mngr;
	}

	virtual void init() {};
	virtual void update() {};
    virtual void receive(const msg::Message& msg) {}

protected:
	ecs::SysIdType id_;
	Manager *mngr_;
	SDLGame *game_;

};

