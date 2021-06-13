#pragma once
#include "ecs.h"
#include "Entity.h"
#include "ObjectPool.h"
#include "Singleton.h"
#include "Transform.h"
#include "AnimatedImageComponent.h"

class FoodPool: public Singleton<FoodPool> {
	friend Singleton<FoodPool> ;
public:
	virtual ~FoodPool() {
	}

	template<typename ...Targs>
	inline static Entity* construct(Targs &&...args) {
		return FoodPool::instance()->construct_(std::forward<Targs>(args)...);
	}

	inline static void destroy(Entity *p) {
		FoodPool::instance()->destroy_(p);
	}

	inline Entity* construct_(const Vector2D &p, double w, double h) {
		Entity *e = pool_.getObj();
		if (e != nullptr) {
			e->setActive(true);
			Transform *tr = e->getComponent<Transform>(ecs::Transform);
			tr->position_ = p;
			tr->width_ = w;
			tr->height_ = h;
		}
		return e;
	}

	inline void destroy_(Entity *p) {
		pool_.relObj(p);
	}

private:
	FoodPool() :
			FoodPool(10) {
	}
	FoodPool(std::size_t n) :
			pool_(n) {
		for (Entity *e : pool_.getPool()) {
			e->addComponent<Transform>();
			auto animatedImage = e->addComponent<AnimatedImageComponent>();
			animatedImage->setFrameTime(100);
			Texture *spritesTex =
					SDLGame::instance()->getTextureMngr()->getTexture(
							Resources::PacManSprites);
			animatedImage->addFrame(spritesTex, { 128*4, 128, 128, 128 });
		}

	}

	ObjectPool<Entity> pool_;
};

