#pragma once

#include <string>
#include <memory>

#include "shader.hpp"
#include "shadow.hpp"

namespace LowRenderer
{
	class ShadowPoint : public Shadow
	{
	private:
		void generateBuffer() override;
		void generateTexture() override;
		void attachTextureToBuffer() override;
	public:
		ShadowPoint();

		void create() override;

		void sendToShader(std::shared_ptr<LowRenderer::Light> light) override;
	};
}