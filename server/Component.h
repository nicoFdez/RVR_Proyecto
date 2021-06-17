#pragma once

#include "ecs.h"

struct Component {
	Component(ecs::CmpIdType id) :
			id_(id) {
	}

	ecs::CmpIdType id_;
};

