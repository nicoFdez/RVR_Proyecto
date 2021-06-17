#pragma once
#include <memory>

/*
 * usage:
 *
 * class A : public Singleton<A> {
 *
 *    friend Singleton<A>; // this way it can call the constructor
 *
 * private: // constructors are private
 *    A() {
 *      //....
 *    }
 *
 * public: // the rest of the functionality
 *    virtual ~A() {
 *    }
 * }
 *
 */
template<typename T>
class Singleton {
protected:
	Singleton() {
	}

public:

	virtual ~Singleton() {
	}

	// some singletons need to be initialized with some parameters, we
	// can call this init method at the begining of the program.
	template<typename ...Targs>
	inline static T* init(Targs &&...args) {
		//assert(instance_.get() == nullptr);
		instance_.reset(new T(std::forward<Targs>(args)...));
		return instance_.get();
	}

	// in some cases, when singletons depend on each other, you have
	// to close them in a specific order, This is why we have this close
	// method
	inline static void close() {
		instance_.reset();
	}

	// get the singleton instance
	//
	inline static T* instance() {
		// you can replace the "if" by assert(instance_.get() != nullptr)
		// to force initialization at the begining
		//
		if (instance_.get() == nullptr) {
			init();
		}
		return instance_.get();
	}

private:
	static std::unique_ptr<T> instance_;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::instance_;

