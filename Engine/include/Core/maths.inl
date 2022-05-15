
namespace Core::Maths
{
	float vec3::squaredMagnitude() const
	{
		return dot(*this, *this);
	}

	float vec3::magnitude() const
	{
		return sqrtf(squaredMagnitude());
	}

	vec3 vec3::normalized() const
    {
		float length = magnitude();

		if (length == 0.f)
			return *this;

		return *this / length;
	}

    vec3& vec3::normalize()
	{
		*this = this->normalized();

		return *this;
	}

	vec3 vectorRotate(const vec3& v, const quat& q)
	{
		vec3 result = { 0 };

		result.x = v.x * (q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z) + v.y * (2 * q.x * q.y - 2 * q.w * q.z) + v.z * (2 * q.x * q.z + 2 * q.w * q.y);
		result.y = v.x * (2 * q.w * q.z + 2 * q.x * q.y) + v.y * (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) + v.z * (-2 * q.w * q.x + 2 * q.y * q.z);
		result.z = v.x * (-2 * q.w * q.y + 2 * q.x * q.z) + v.y * (2 * q.w * q.x + 2 * q.y * q.z) + v.z * (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);

		return result;
	}

	vec3 reflect(const vec3& toReflect, const vec3& normal)
	{
		return toReflect - 2.f * normal * dot(toReflect, normal);
	}


	void quat::getAxisAngle(vec3& axis, float& angle) const
	{
		/*quat normalizedQuat = *this;

		if (fabs(w) > 1.0f)
			normalizedQuat = normalized();

		vec3 resAxis;
		float den = sqrtf(1.0f - w * w);

		if (den > 0.0001f)
			axis = vec3(x, y, z) / den;	// Axis
		else
			axis.x = 1.0f;		// Default axis

		angle = 2.0f * acosf(w);*/

		quat normalizedQuat = *this;
		if (fabs(normalizedQuat.w) > 1.0f)
			normalizedQuat = normalizedQuat.normalized();

		vec3 resAxis = { 0.0f, 0.0f, 0.0f };
		float resAngle = 2.0f * acosf(normalizedQuat.w);
		float den = sqrtf(1.0f - normalizedQuat.w * normalizedQuat.w);

		if (den > 0.0001f)
		{
			resAxis.x = normalizedQuat.x / den;
			resAxis.y = normalizedQuat.y / den;
			resAxis.z = normalizedQuat.z / den;
		}
		else
		{
			// This occurs when the angle is zero.
			// Not a problem: just set an arbitrary normalized axis.
			resAxis.x = 1.0f;
		}

		axis = resAxis;
		angle = resAngle;
	}

	float quat::length() const
	{
		//return sqrtf(x * x + y * y + z * z + w * w);
		float result = (float)sqrt(x * x + y * y + z * z + w * w);
		return result;
	}

	quat quat::normalized() const
	{
		/*float _length = length();

		if (_length == 0.0f)
			return *this;

		float invLength = 1.0f / _length;

		return {
			x * invLength,
			y * invLength,
			z * invLength,
			w * invLength
		};*/

		quat result = quat(0.f, 0.f, 0.f, 0.f);

		float magn, ilength;
		magn = length();
		if (magn == 0.0f) magn = 1.0f;
		ilength = 1.0f / magn;

		result.x = x * ilength;
		result.y = y * ilength;
		result.z = z * ilength;
		result.w = w * ilength;

		return result;
	}

	quat quat::identity()
	{
		return { 0.f, 0.f, 0.f, 1.f };
	}
}
