#pragma once

#include "maths.hpp"

namespace Physics
{
	class Cylinder
	{
	public:
		Cylinder()
			: ptA({ 0.f, 0.f, 0.f }), ptB({ 0.f, 0.f, 0.f }), radius(0.f) {}

		Cylinder(const vec3& axe0, const vec3& axe1, float rad, const quat& _q)
			: ptA(axe0), ptB(axe1), radius(rad), quaternion(_q) {}

		Cylinder(const vec3& origin, float height, float _radius, quat q)
			: radius(_radius)
		{
			quaternion = q;
			vec3 axis = vectorRotate({ 0,1,0 }, quaternion) * height;
			ptA = origin - axis * 0.5f;
			ptB = origin + axis * 0.5f;
		}

		vec3 ptA;
		vec3 ptB;
		quat quaternion = quat::identity();
		float radius;
	};
}