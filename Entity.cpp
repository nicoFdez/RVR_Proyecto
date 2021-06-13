#include "Entity.h"
#include "Manager.h"

void Entity::addToGroup(ecs::GrpIdType id) {
	groups_[id] = true;
	mngr_->addToGroup(id,this);
}
