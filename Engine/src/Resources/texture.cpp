#include "texture.hpp"

#include <imgui.h>
#include <filesystem>

#include "stb_image.h"
#include "define.hpp"

#include "debug.hpp"
#include "graph.hpp"

namespace Resources
{
	 std::shared_ptr<Texture> Texture::defaultAlpha = nullptr;
	 std::shared_ptr<Texture> Texture::defaultAmbient = nullptr;
	 std::shared_ptr<Texture> Texture::defaultDiffuse = nullptr;
	 std::shared_ptr<Texture> Texture::defaultEmissive = nullptr;
	 std::shared_ptr<Texture> Texture::defaultSpecular = nullptr;

	bool Texture::load(const std::string& filePath)
	{
		stbi_set_flip_vertically_on_load_thread(true);

		int channel = 0;
		// Get the color buffer by using stbi
		colorBuffer = stbi_loadf(filePath.c_str(), &width, &height, &channel, STBI_rgb_alpha);

		return colorBuffer;
	}

	void Texture::create(const std::string& filePath, bool isMulti)
	{
		if (load(filePath))
		{
			m_filePath = filePath;

			if (isMulti)
				Core::Engine::Graph::addRequest(std::bind(&Texture::generate, this));
			else
				generate();

			return;
		}

		// Error on load (missing textures or fail open file)
		const char* error = stbi_failure_reason();
		std::string errorStr = error + ' ';

		Core::Debug::Log::error(error + filePath);
	}

	void Texture::set(const std::string& filePath, int _width, int _height, float* _colorBuffer)
	{
		if (!_colorBuffer)
		{
			Core::Debug::Log::error("Texture " + m_filePath + " buffer is not valid");
			return;
		}

		width = _width;
		height = _height;
		colorBuffer = _colorBuffer;
		m_filePath = filePath;

		generateID();
	}

	Texture::~Texture()
	{
		if (textureID)
			glDeleteTextures(1, &textureID);
	}

	void Texture::generate()
	{
		if (!colorBuffer)
		{
			Core::Debug::Log::error("Texture " + m_filePath + " buffer is not valid");
			return;
		}

		generateID();
		stbi_image_free(colorBuffer);
	}

	void Texture::generateID()
	{
		// Generate the texture ID
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set the texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, colorBuffer);

		// Generate its mipmap
		//glGenerateMipmap(GL_TEXTURE_2D);

		isDrawable = true;

		Core::Engine::Benchmark::addTexturesCount(1);
	}

	GLuint Texture::getID() const
	{
		return textureID;
	}

	void Texture::bind(int textureIndex) const
	{
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
}