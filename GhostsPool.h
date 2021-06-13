#pragma once
#include "ecs.h"
#include "Entity.h"
#include "ObjectPool.h"
#include "Singleton.h"
#include "Transform.h"
#include "AnimatedImageComponent.h"

class GhostsPool: public Singleton<GhostsPool> {
	friend Singleton<GhostsPool> ;
public:
	virtual ~GhostsPool() {
	}

	template<typename ...Targs>
	inline static Entity* construct(Targs &&...args) {
		return GhostsPool::instance()->construct_(std::forward<Targs>(args)...);
	}

	inline static void destroy(Entity *p) {
		GhostsPool::instance()->destroy_(p);
	}

	inline Entity* construct_(const Vector2D &p, const Vector2D &v, double w, double h) {
		Entity *e = pool_.getObj();
		if (e != nullptr) {
			e->setActive(true);
			Transform *tr = e->getComponent<Transform>(ecs::Transform);
			tr->position_ = p;
			tr->velocity_ = v;
			tr->width_ = w;
			tr->height_ = h;
		}
		return e;
	}

	inline void destroy_(Entity *p) {
		pool_.relObj(p);
	}

private:
	GhostsPool() :
			GhostsPool(10) {
	}
	GhostsPool(std::size_t n) :
			pool_(n) {
		for (Entity *e : pool_.getPool()) {
			e->addComponent<Transform>();
			auto animatedImage = e->addComponent<AnimatedImageComponent>();
			animatedImage->setFrameTime(100);
			Texture *spritesTex =
					SDLGame::instance()->getTextureMngr()->getTexture(
							Resources::PacManSprites);
			for (int i = 0; i < 4; i++) {
				animatedImage->addFrame(spritesTex, { i * 128, 768, 128, 128 });
			}

		}

	}

	ObjectPool<Entity> pool_;
};

