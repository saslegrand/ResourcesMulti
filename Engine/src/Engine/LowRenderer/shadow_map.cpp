#include "shadow_map.hpp"

#include "resources_manager.hpp"
#include "render_manager.hpp"
#include "debug.hpp"

namespace LowRenderer
{
	ShadowMap::ShadowMap()
		: Shadow("depthShader")
	{
		Core::Debug::Log::info("Create ShadowMap");

		create();
	}

	void ShadowMap::generateBuffer()
	{
		glGenFramebuffers(1, &FBO);
	}

	void ShadowMap::generateTexture()
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Clamp to border for deactivate frustum shadows
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	void ShadowMap::attachTextureToBuffer()
	{
		Core::Debug::Log::info("ShadowMap : Attach texture and depth buffer");

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowMap::create()
	{
		generateBuffer();
		generateTexture();
		attachTextureToBuffer();
	}

	void ShadowMap::sendToShader(std::shared_ptr<LowRenderer::Light> light)
	{
		program->setUniform("lightSpaceMatrix", light->getSpaceMatrix().e, 1, 1);
	}
}