#include "Manager.h"

#include <algorithm>

void Manager::refresh() {

	// execute all events
	while (!events_.empty()) {
		auto &event = events_.front();
		event();
		events_.pop_front();
	}

	// update groups
	for (auto i(0u); i < ecs::maxGroups; i++) {
		auto &grp = entsGroups_[i];
		grp.erase( //
				std::remove_if( //
						grp.begin(),  //
						grp.end(), //
						[i](Entity *e) { //
							return !e->isActive() || !e->hasGroup(i);
						}), //
				grp.end());
	}

	// update the list of enteties
	ents_.erase( //
			std::remove_if( //
					ents_.begin(),  //
					ents_.end(), //
					[](const uptr_ent &e) { //
						return !e->isActive();
					}), //
			ents_.end());
}

