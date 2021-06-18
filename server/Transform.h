#pragma once

#include "Vector2D.h"
#include "Component.h"

struct Transform: Component {
	Transform(Vector2D pos, Vector2D vel, double width, double height,
			double rotation) :
			Component(ecs::Transform), //
			position_(pos), //
			width_(width), //
			height_(height), //
			rotation_(0.0) //
	{
	}

	Transform() :
			Component(ecs::Transform), //
			position_(), //
			width_(), //
			height_(), //
			rotation_() //
	{
	}

	Vector2D position_;
	double width_;
	double height_;
	double rotation_;
};

