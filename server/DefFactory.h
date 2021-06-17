#pragma once
#include <utility>

template<typename T>
class DefFactory {
public:
	DefFactory() {
	}
	virtual ~DefFactory() {
	}

	template<typename ...Targs>
	inline static T* construct(Targs&& ...args) {
		return new T(std::forward<Targs>(args)...);
	}

	inline static void destroy(T* p) {
		delete p;
	}

};

