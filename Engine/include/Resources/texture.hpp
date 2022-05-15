#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>

#include "resource.hpp"

#include "maths.hpp"

namespace Resources
{
	class Texture : public Resource
	{
	private:
		GLuint textureID = 0;

		int width = 0;
		int height = 0;
		float* colorBuffer = nullptr;
		
		void generateID();
		bool load(const std::string& filePath);

	public:
		Texture() = default;
		~Texture();

		bool isDrawable = false;

		GLuint getID() const;

		void generate();
		void bind(int textureIndex) const;
		void create(const std::string& filePath, bool isMulti);
		void set(const std::string& filePath, int width, int height, float* colorBuffer);

		static std::shared_ptr<Texture> defaultAlpha;
		static std::shared_ptr<Texture> defaultAmbient;
		static std::shared_ptr<Texture> defaultDiffuse;
		static std::shared_ptr<Texture> defaultEmissive;
		static std::shared_ptr<Texture> defaultSpecular;
	};	
}
