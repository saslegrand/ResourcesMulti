#pragma once

#include "renderer.hpp"
#include "model.hpp"
#include "transform.hpp"
#include "collider.hpp"

#define MAT_COLLIDER_COLOR Core::Maths::vec3(0.f, 1.f, 0.f)

namespace LowRenderer
{
	class ColliderRenderer : public Renderer
	{
	private:
		LowRenderer::Model model;
		std::shared_ptr<Physics::Collider> collider = nullptr;

	public:
		ColliderRenderer(Engine::GameObject& gameObject, std::shared_ptr<Physics::Collider> ptr, const std::string& modelFilePath);
		~ColliderRenderer() {}

		Core::Maths::mat4 getModelCollider() const;
		bool canBeDraw();
		void draw() const override;
		void drawImGui() override;
	};
}