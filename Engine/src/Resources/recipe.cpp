#include "recipe.hpp"

#include "debug.hpp"

namespace Resources
{
	Recipe::Recipe(const std::string& filePath)
		: Resource(filePath)
	{
        std::ifstream ifs(filePath);

        // Check if the file can be read
        if (ifs.fail()) Core::Debug::Log::error("Cannot load the recipe: " + filePath);

        recipe.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	}
}