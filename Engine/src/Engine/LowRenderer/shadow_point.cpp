#include "shadow_point.hpp"

#include "debug.hpp"
#include "resources_manager.hpp"

namespace LowRenderer
{
	ShadowPoint::ShadowPoint()
		: Shadow("depthCubeShader")
	{
		Core::Debug::Log::info("Create ShadowPoint");

		create();
	}

	void ShadowPoint::generateBuffer()
	{
		glGenFramebuffers(1, &FBO);
	}

	void ShadowPoint::generateTexture()
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
		for (int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
						 shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void ShadowPoint::attachTextureToBuffer()
	{
		Core::Debug::Log::info("ShadowPoint : Attach texture and depth buffer");

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowPoint::create()
	{
		generateBuffer();
		generateTexture();
		attachTextureToBuffer();
	}

	void ShadowPoint::sendToShader(std::shared_ptr<LowRenderer::Light> light)
	{
		Core::Maths::vec3 lightPos = light->position.xyz;
		float farPlane = 25.f;

		Core::Maths::mat4 shadowProjection = Core::Maths::perspective(Core::Maths::DEG2RAD * 90.f, getAspect(), 0.001f, farPlane);
		std::vector<Core::Maths::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProjection * Core::Maths::lookAt(lightPos, lightPos + Core::Maths::vec3(1.f, 0.f, 0.f),
			Core::Maths::vec3(0.f, -1.f, 0.f)));
		shadowTransforms.push_back(shadowProjection * Core::Maths::lookAt(lightPos, lightPos + Core::Maths::vec3(-1.f, 0.f, 0.f),
			Core::Maths::vec3(0.f, -1.f, 0.f)));
		shadowTransforms.push_back(shadowProjection * Core::Maths::lookAt(lightPos, lightPos + Core::Maths::vec3(0.f, 1.f, 0.f),
			Core::Maths::vec3(0.f, 0.f, 1.f)));
		shadowTransforms.push_back(shadowProjection * Core::Maths::lookAt(lightPos, lightPos + Core::Maths::vec3(0.f, -1.f, 0.f),
			Core::Maths::vec3(0.f, 0.f, -1.f)));
		shadowTransforms.push_back(shadowProjection * Core::Maths::lookAt(lightPos, lightPos + Core::Maths::vec3(0.f, 0.f, 1.f),
			Core::Maths::vec3(0.f, -1.f, 0.f)));
		shadowTransforms.push_back(shadowProjection * Core::Maths::lookAt(lightPos, lightPos + Core::Maths::vec3(0.f, 0.f, -1.f),
			Core::Maths::vec3(0.f, -1.f, 0.f)));

		for (int i = 0; i < 6; i++)
			program->setUniform("shadowMatrices[" + std::to_string(i) + "][0]", shadowTransforms[i].e, 1, 1);

		program->setUniform("farPlane", &farPlane);
		program->setUniform("lightPos", &lightPos);
	}
}