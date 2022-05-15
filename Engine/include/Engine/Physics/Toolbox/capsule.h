#pragma once

#include "maths.hpp"

namespace Physics
{
	class Capsule
	{
	public:
		Capsule() = default;

		Capsule(const vec3& _ptA, const vec3& _ptB, float _radius, const quat& _quaternion)
			: ptA(_ptA), ptB(_ptB), radius(_radius), quaternion(_quaternion) {}

		Capsule(const vec3& origin, float height, float _radius, const quat& q)
		{
			quaternion = q;
			vec3 axis = vectorRotate({ 0,1,0 }, quaternion) * height;
			ptA = origin - axis * 0.5f;
			ptB = origin + axis * 0.5f;
			radius = _radius;
		}

		vec3 ptA;
		vec3 ptB;
		quat quaternion = quat::identity();
		float radius;
	};
}