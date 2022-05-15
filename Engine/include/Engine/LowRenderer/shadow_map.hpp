#pragma once

#include "shadow.hpp"

namespace LowRenderer
{
	class ShadowMap : public Shadow
	{
	private:
		void generateBuffer() override;
		void generateTexture() override;
		void attachTextureToBuffer() override;
	public:
		ShadowMap();

		void create() override;

		void sendToShader(std::shared_ptr<LowRenderer::Light> light) override;
	};
}