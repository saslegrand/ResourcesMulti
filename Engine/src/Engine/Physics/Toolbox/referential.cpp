#include "referential.h"

Referential3D::Referential3D()
{
	origin = vec3(0.f, 0.f, 0.f);
	i = vec3(1.f, 0.f, 0.f);
	j = vec3(0.f, 1.f, 0.f);
	k = vec3(0.f, 0.f, 1.f);
}

Referential3D::Referential3D(const vec3& ptA, const vec3& ptB)
{
	vec3 axe = ptB - ptA;

	origin = ptA + (axe / 2.f);
	j = axe.normalized();
	vec3 notJ = { 1.f, 0.f, 0.f };
	if (j == notJ)
		notJ = { 0.f, 1.f, 0.f };
	k = (j ^ notJ).normalized();
	i = (k ^ j).normalized();
}

Referential3D::Referential3D(const vec3& _origin, const quat& quaternion)
	: origin(_origin)
{
	i = vectorRotate({ 1,0,0 }, quaternion).normalized();
	j = vectorRotate({ 0,1,0 }, quaternion).normalized();
	k = vectorRotate({ 0,0,1 }, quaternion).normalized();
}

// Normalize referential vectors
void Referential3D::normalize()
{
	i.normalize();
	j.normalize();
	k.normalize();
}

vec3 Referential3D::localToGlobalRef(vec3 localPos)
{
	return origin + localToGlobalVectRef(localPos);
}

vec3 Referential3D::globalToLocalRef(vec3 globalPos)
{
	return globalToLocalVectRef(globalPos - origin);
}

vec3 Referential3D::localToGlobalVectRef(vec3 localPos)
{
	return (i * localPos.x) + (j * localPos.y) + (k * localPos.z);
}

vec3 Referential3D::globalToLocalVectRef(vec3 globalPos)
{
	return vec3(dot(globalPos, i), dot(globalPos, j), dot(globalPos, k));
}