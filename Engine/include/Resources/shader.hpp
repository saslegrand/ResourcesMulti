#pragma once

#include <unordered_map>

#include <glad/glad.h>

#include "resource.hpp"

namespace Resources
{
	struct Uniform
	{
		GLint  location;
		GLenum type;
	};

	class Shader : public Resource
	{
	private:
		std::string loadFromFile(const std::string& filePath);

	public:
		GLint shaderID = GL_INVALID_VALUE;

		Shader(const std::string& shaderPath);
		~Shader();

		void load(const std::string& path);
	};

	class ShaderProgram : public Resource
	{
	private:
		GLint programID = GL_INVALID_VALUE;
		std::string name;

		std::unordered_map<std::string, Uniform> uniforms;

		void loadLocations();

	public:
		ShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
		~ShaderProgram();

		void setUniform(const std::string& target, const void* value, int count = 1, bool transpose = false) const;
		void bind() const;
		void unbind() const;

		std::string getName();
	};
}