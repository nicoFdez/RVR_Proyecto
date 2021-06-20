#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <functional>

#include "Component.h"
#include "SDLGame.h"
#include "ecs.h"
#include "DefFactory.h"

class Manager;


/*
* Clase que va a representar las entidades dentro de nuestro juego y va a contener componentes
*/
class Entity {
	// defining a type just for brevity
	using uptr_cmp = std::unique_ptr<Component, std::function<void(Component*)>>;

public:
	Entity() :
		game_(SDLGame::instance()), //
		mngr_(nullptr), //
		active_(true) {
	}

	virtual ~Entity() {
	}

	inline Manager* getEntityMngr() const {
		return mngr_;
	}

	inline void setEntityMngr(Manager* mngr) {
		mngr_ = mngr;
	}

	inline bool isActive() const {
		return active_;
	}

	inline void setActive(bool active) {
		active_ = active;
	}

	template<typename T>
	inline T* getComponent(ecs::CmpIdType id) {
		return static_cast<T*>(cmpArray_[id].get());
	}

	template<typename T>
	inline bool hasComponent(ecs::CmpIdType id) {
		return cmpArray_[id] != nullptr;
	}

	template<typename T, typename FT = DefFactory<T>, typename ... Targs>
	inline T* addComponent(Targs &&...args) {
		// create the actual component ...
		T* c = FT::construct(std::forward<Targs>(args)...);
		uptr_cmp uPtr(c, [](Component* p) {
			FT::destroy(static_cast<T*>(p));
		});

		// store it in the components array
		cmpArray_[c->id_] = std::move(uPtr);

		return c;
	}

	template<typename T>
	inline void removeComponent(ecs::CmpIdType id) {
		cmpArray_[id] = nullptr;
	}

private:
	SDLGame* game_;
	Manager* mngr_;

	std::array<uptr_cmp, ecs::maxComponents> cmpArray_ = { };

	bool active_;
};
