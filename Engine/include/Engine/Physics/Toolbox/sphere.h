#pragma once

#include "shape.hpp"
namespace Physics
{
	class Sphere : public Shape
	{
	public:
		Sphere() = default;

		Sphere(const vec3& _origin, float _radius, const quat& _quaternion = quat::identity())
			: Shape(_origin, _quaternion), radius(_radius) {}

		float radius = 1.f;
	};
}