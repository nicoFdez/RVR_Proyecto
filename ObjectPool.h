#pragma once
#include <cassert>
#include <vector>
#include <list>

template<typename T>
class ObjectPool {
public:
	ObjectPool(std::size_t size) {
		size_ = size;
		objs_ = new T[size_];
		for (auto i(0u); i < size_; i++) {
			objsPtrs_.push_back(&objs_[i]);
			free_.push_back(&objs_[i]);
		}
	}

	virtual ~ObjectPool() {
		delete[] objs_;
	}

	T* getObj() {
		if (!free_.empty()) {
			T* o = free_.front();
			free_.pop_front();
			return o;
		} else
			return nullptr;
	}

	void relObj(T* p) {
		std::size_t idx = p-objs_;
		assert(idx >=0 && idx<size_);
		free_.push_back(p);
	}

	const std::vector<T*>& getPool() {
		return objsPtrs_;
	}

private:
	std::size_t size_;
	T *objs_;
	std::vector<T*> objsPtrs_;
	std::list<T*> free_;
};

