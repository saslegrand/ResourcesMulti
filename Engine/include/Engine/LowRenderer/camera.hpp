#pragma once

#include "component.hpp"

#include "maths.hpp"
#include "shader.hpp"

namespace Physics
{
	class Transform;
};

namespace LowRenderer
{
	class Camera : public Engine::Component
	{
	private:
		Camera(Engine::GameObject& gameObject, const std::shared_ptr<Camera>& ptr);

		std::shared_ptr<Physics::Transform> m_transform = nullptr;

	public:
		float aspect = 0.f;
		float near = 0.001f;
		float far = 200.f;
		float fovY = 60.f;

		Camera(Engine::GameObject& gameObject);

		Core::Maths::mat4 getViewMatrix() const;
		Core::Maths::mat4 getProjection() const;
		Core::Maths::mat4 getOrthographic() const;
		Core::Maths::mat4 getShadowOrtho() const;
		Core::Maths::mat4 getViewProjection() const;
		Core::Maths::mat4 getViewOrthographic() const;

		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		void sendViewProjToProgram(const std::shared_ptr<Resources::ShaderProgram> program);
		void sendProjToProgram(const std::shared_ptr<Resources::ShaderProgram> program);
		void sendViewOrthoToProgram(const std::shared_ptr<Resources::ShaderProgram> program);

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}