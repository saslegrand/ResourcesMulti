#pragma once

#include <fstream>

#include "resource.hpp"

namespace Resources
{
	struct Recipe : public Resource
	{
		Recipe(const std::string& filePath);

		std::string recipe;
	};
}