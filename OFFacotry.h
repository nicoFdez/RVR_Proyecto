#pragma once
#include "ObjectFactory.h"
#include "Singleton.h"

template<typename T>
class OFFacotry: public Singleton<OFFacotry<T>> {
	friend Singleton<OFFacotry<T>> ;
public:
	virtual ~OFFacotry() {
	}

	template<typename ...Targs>
	inline static T* construct(Targs &&...args) {
		return OFFacotry<T>::instance()->construct_(
				std::forward<Targs>(args)...);
	}

	inline static void destroy(T *p) {
		OFFacotry<T>::instance()->destroy_(p);
	}

	template<typename ...Targs>
	inline T* construct_(Targs &&...args) {
		return pool_.construct(std::forward<Targs>(args)...);
	}

	inline void destroy_(T *p) {
		pool_.destroy(p);
	}

private:
	OFFacotry() :
			OFFacotry(10) {
	}
	OFFacotry(std::size_t n) :
			pool_(n) {
	}

	ObjectFactory<T> pool_;

};

