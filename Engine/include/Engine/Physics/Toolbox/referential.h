#pragma once

#include "maths.hpp"

using namespace Core::Maths;

class Referential3D
{
public:
	Referential3D();

	Referential3D(const vec3& axe0, const vec3& axe1);

	Referential3D(const vec3& _origin, const vec3& _i, const vec3& _j, const vec3& _k)
		: origin(_origin), i(_i), j(_j), k(_k) {}

	Referential3D(const vec3& _origin, const quat& quaternion);

	void normalize();

	vec3      localToGlobalRef(vec3 localPos);
	vec3      globalToLocalRef(vec3 globalPos);
	vec3      localToGlobalVectRef(vec3 localPos);
	vec3      globalToLocalVectRef(vec3 globalPos);

	vec3 origin;
	vec3 i;
	vec3 j;
	vec3 k;
};