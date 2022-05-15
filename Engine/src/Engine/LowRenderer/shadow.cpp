#include "shadow.hpp"

#include "resources_manager.hpp"
#include "application.hpp"

namespace LowRenderer
{
	Shadow::Shadow(const std::string& shaderProgramName)
	{
		shadowWidth = (unsigned int)((float)shadowWidth * superSample);
		shadowHeight = (unsigned int)((float)shadowHeight * superSample);

		aspect = (float)shadowWidth / (float)shadowHeight;

		program = Resources::ResourcesManager::loadShaderProgram(shaderProgramName);
	}

	Shadow::~Shadow()
	{
		if (FBO)
			glDeleteFramebuffers(1, &FBO);
		if (ID)
			glDeleteTextures(1, &ID);
	}

	float Shadow::getAspect()
	{
		return aspect;
	}

	void Shadow::bindAndSetViewport()
	{
		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}

	void Shadow::unbindAndResetViewport()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Core::Maths::vec2 windowSize = Core::Application::getWindowSize();
		glViewport(0, 0, (GLsizei)windowSize.x, (GLsizei)windowSize.y);
	}
}