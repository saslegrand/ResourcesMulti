#pragma once

#include "maths.hpp"

namespace Physics
{
	struct Rect2D
	{
		Rect2D(Core::Maths::vec2 _center, float _halfWidth, float _halfHeight)
			: center(_center), halfWidth(_halfWidth), halfHeight(_halfHeight) {}

		Rect2D() = default;

		Core::Maths::vec2 center;
		
		float halfWidth = 0.f;
		float halfHeight = 0.f;
	};
}