#pragma once
#include "maths.hpp"

namespace LowRenderer
{
	struct Color
	{
		Color(float _r = 0.f, float _g = 0.f, float _b = 0.f, float _a = 1.f)
			: data(_r, _g, _b, _a) {}
		Color(Core::Maths::vec3 _rgb, float _a = 1.f)
			: data(_rgb, _a) {}

		Core::Maths::vec4 data;

		// Primary colors
		static Color white;
		static Color black;
		static Color red;
		static Color blue;
		static Color green;
	};
}
