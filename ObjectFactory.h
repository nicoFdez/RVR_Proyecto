#pragma once
#include <cassert>
#include <cstdlib>
#include <memory>
#include <new>

template<typename T, typename AllocType = std::allocator<T>>
class ObjectFactory {
public:
	ObjectFactory(int n) {

		size_ = n;
		//mem_ = static_cast<T*>(_VSTD::__libcpp_allocate(size_ * sizeof(T), _LIBCPP_ALIGNOF(T)));
		mem_ = alloc_.allocate(size_);
		used_ = new bool[size_]();
	}

	virtual ~ObjectFactory() {
        //_VSTD::__libcpp_deallocate((void*)mem_, size_ * sizeof(T), _LIBCPP_ALIGNOF(T));
		alloc_.deallocate(mem_,size_);
	}

	template<typename... Targs>
	T* construct(Targs&& ...args) {
		for(int i=0; i<size_; i++) {
			if ( !used_[i] ) {
				used_[i] = true;
				//::new(&mem_[i]) T(std::forward<Targs>(args)...);
				alloc_.construct(&mem_[i],std::forward<Targs>(args)...);
				return &mem_[i];
			}
		}
		return nullptr;
	}

	void destroy(T* p) {
		int idx = p-mem_;
		assert(idx >=0 && idx < size_);
		used_[idx] = false;
//		p->~T();
		alloc_.destroy(p);
	}

private:
	std::size_t size_;
	T* mem_;
	bool *used_;
	AllocType alloc_;
};

