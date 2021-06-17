#include "Manager.h"

#include <algorithm>

void Manager::refresh() {
	// execute all events
	while (!events_.empty()) {
		auto& event = events_.front();
		event();
		events_.pop_front();
	}

	// update the list of enteties
	ents_.erase( //
		std::remove_if( //
			ents_.begin(),  //
			ents_.end(), //
			[](const uptr_ent& e) { //
		return !e->isActive();
	}), //
		ents_.end());
}