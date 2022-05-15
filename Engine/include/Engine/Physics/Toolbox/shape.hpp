#pragma once
#include "maths.hpp"
#include "transform.hpp"

namespace Physics
{
	using namespace Core::Maths;

	class Shape
	{
	protected:
		Shape() = default;
		Shape(const vec3& origin, const quat& _quaternion = quat::identity());
	public:
		vec3 center;
		quat quaternion = quat::identity();
	};
}