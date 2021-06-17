#pragma once

#include "Component.h"
#include "Texture.h"

struct ImageComponent: Component {
	ImageComponent(Texture *tex) :
			Component(ecs::ImageComponent), //
			tex_(tex) //
	{
	}
	Texture *tex_;
};

