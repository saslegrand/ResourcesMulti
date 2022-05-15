#include "cube_map.hpp"

#include "stb_image.h"

#include "graph.hpp"
#include "define.hpp"
#include "debug.hpp"

namespace Resources
{
	CubeMap::~CubeMap()
	{
		if (ID)
			glDeleteTextures(1, &ID);
	}

	void CubeMap::create(const std::vector<std::string>& paths, bool isMulti)
	{
		stbi_set_flip_vertically_on_load_thread(false);

		int channel;
		unsigned int dimIndex, dimIndexUp;

		for (unsigned int i = 0; i < 6; i++)
		{
			dimIndex = i * 2;
			dimIndexUp = dimIndex + 1;

			std::string absPath = PROJECT_DIR + paths[i];
			datas[i] = stbi_load(absPath.c_str(), &dimensions[dimIndex], &dimensions[dimIndexUp], &channel, 0);

			if (!datas[i])
			{
				// Error on load (missing textures or fail open file)
				const char* error = stbi_failure_reason();
				std::string errorStr = error + ' ';

				Core::Debug::Log::error(error + paths[i]);
				continue;
			}

			Core::Debug::Log::info("Loading of " + paths[i] + " done with success");
		}

		if (isMulti)
		{
			Core::Engine::Graph::addRequest(std::bind(&CubeMap::generate, this));
			return;
		}

		generate();
	}

	void CubeMap::generate()
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		unsigned int dimIndex, dimIndexUp;

		for (unsigned int i = 0; i < 6; i++)
		{
			dimIndex = i * 2;
			dimIndexUp = dimIndex + 1;

			if (datas[i])
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, dimensions[dimIndex], dimensions[dimIndexUp], 0, GL_RGBA, GL_UNSIGNED_BYTE, datas[i]);
				stbi_image_free(datas[i]);
			}

		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		Core::Engine::Benchmark::addTexturesCount(6);

		isDrawable = true;
	}

	GLuint CubeMap::getID() const
	{
		return ID;
	}

	void CubeMap::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}
}