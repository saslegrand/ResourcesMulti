#pragma once

#include <memory>

#include "shader.hpp"
#include "light.hpp"

namespace LowRenderer
{
	class Shadow
	{
	protected:
		Shadow(const std::string& shaderProgramName);
		Shadow() = default;

		unsigned int shadowWidth = 2048;
		unsigned int shadowHeight = 2048;
		float superSample = 4.f;
		float aspect = 0.f;

		GLuint FBO = 0;

	public:
		~Shadow();

		// Depth map
		unsigned int ID = 0;
		std::shared_ptr<Resources::ShaderProgram> program = nullptr;

		void virtual generateBuffer() = 0;
		void virtual generateTexture() = 0;
		void virtual attachTextureToBuffer() = 0;

		void virtual create() = 0;

		void virtual sendToShader(std::shared_ptr<LowRenderer::Light> light) = 0;

		void bindAndSetViewport();
		void unbindAndResetViewport();

		float getAspect();
	};
}
