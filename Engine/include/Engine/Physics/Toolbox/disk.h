#pragma once

#include "shape.hpp"

namespace Physics
{
	class Disk : public Shape
	{
	public:
		Disk() = default;
		Disk(const vec3& _normal, const vec3& _center, float _radius) 
			: Shape(_center), normal(_normal), radius(_radius) {}

		vec3  normal;
		float radius;
	};
}