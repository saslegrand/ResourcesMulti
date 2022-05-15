#pragma once

#include "shape.hpp"

namespace Physics
{
	class Quad : public Shape
	{
	public:
		Quad() = default;
		Quad(const vec3& _origin, const vec3& _extensions, const quat& _quaternion = quat::identity())
			: Shape(_origin, _quaternion), size(_extensions) {}

		vec3 size = vec3(1.f, 1.f, 1.f);
	};
}