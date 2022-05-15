#pragma once

#include "shape.hpp"

namespace Physics
{
	class Box : public Shape
	{
	public:
		Box() = default;
		Box(const Core::Maths::vec3& _origin, const Core::Maths::vec3& _extensions, float _offsetRounding, const quat& q = quat::identity())
			: Shape(_origin, q), offsetRounding(_offsetRounding), size(_extensions) {}

		float offsetRounding = 0.f;
		vec3 size = vec3(1.f, 1.f, 1.f);
	};
}