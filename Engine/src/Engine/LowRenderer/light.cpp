#include "light.hpp"

#include "imgui.h"

#include "render_manager.hpp"
#include "resources_manager.hpp"
#include "inputs_manager.hpp"
#include "time.hpp"

#include "transform.hpp"
#include "shadow_point.hpp"
#include "shadow_map.hpp"
#include "utils.hpp"

namespace LowRenderer
{
	Light::Light(Engine::GameObject& gameObject)
		: Light(gameObject, std::shared_ptr<Light>(this))
	{
		m_transform = requireComponent<Physics::Transform>();
		shadow = std::make_unique<ShadowMap>();
	}

	Light::Light(Engine::GameObject& gameObject, const std::shared_ptr<Light>& ptr)
		: Component(gameObject, ptr)
	{
		LowRenderer::RenderManager::linkComponent(ptr);
	}

	void Light::setAsDirectionnal()
	{
		position.w = 0.f;

		cutoff = Core::Maths::PI;
		outterCutoff = Core::Maths::PI;
	}

	void Light::setAsPoint()
	{
		position.w = 1.f;

		cutoff = Core::Maths::PI;
		outterCutoff = Core::Maths::PI;
	}

	void Light::setAsSpot()
	{
		position.w = 1.f;

		cutoff = Core::Maths::PIO4;
		outterCutoff = 50.f * Core::Maths::DEG2RAD;
	}

	void Light::setShadows(bool isShadow)
	{
		hasShadow = (float)isShadow;

		if (isShadow)
		{
			bool isShadowMap = dynamic_cast<ShadowMap*>(shadow.get()) != nullptr;

			if (!isShadowMap && position.w == 0.0)
				shadow = std::make_unique<ShadowMap>();
			else if (!isShadowMap && dynamic_cast<ShadowPoint*>(shadow.get()) == nullptr)
				shadow = std::make_unique<ShadowPoint>();

			return;
		}

		shadow = nullptr;
	}

	void Light::compute()
	{
		enable = (float)isActive();
		hasShadow = (float)(shadow != nullptr);
		position.xyz = m_transform->m_position;

		if (hasShadow == 0.f || position.w != 0.f)
			return;

		Core::Maths::mat4 lightView = Core::Maths::lookAt(position.xyz, Core::Maths::vec3(), Core::Maths::vec3(0.f, 1.f, 0.f));
		spaceMatrix = LowRenderer::RenderManager::getCurrentCamera()->getShadowOrtho() * lightView;
	}

	void Light::sendToProgram(std::shared_ptr<Resources::ShaderProgram> program, int index) const
	{
		// Send light parameters to the ShaderProgram packed into matrices
		program->setUniform("lightAttribs1[" + std::to_string(index) + "][0]", &position);
		program->setUniform("lightAttribs2[" + std::to_string(index) + "][0]", &attenuation);

		if (shadow != nullptr)
		{
			int test = 0;
			if (position.w == 0.f)
			{
				test = 5 + index;
				program->setUniform("lightAttribs3[" + std::to_string(index) + "][0]", &spaceMatrix.e, 1, 1);
				program->setUniform("shadowMaps[" + std::to_string(index) + "][0]", &test);

				glActiveTexture(GL_TEXTURE5 + index);
				glBindTexture(GL_TEXTURE_2D, shadow->ID);
			}
			else
			{
				test = 13 + index;
				float farPlane = 25.f;
				program->setUniform("farPlane", &farPlane);
				program->setUniform("shadowCubeMaps[" + std::to_string(index) + "][0]", &test);

				glActiveTexture(GL_TEXTURE13 + index);
				glBindTexture(GL_TEXTURE_CUBE_MAP, shadow->ID);
			}
		}
	}

	const Core::Maths::mat4& Light::getSpaceMatrix() const
	{
		return spaceMatrix;
	}

	void Light::drawImGui()
	{
		if (ImGui::TreeNode("Light"))
		{
			ImGui::ColorEdit3("Ambient: ", &ambient.data.x);
			ImGui::ColorEdit3("Diffuse: ", &diffuse.data.x);
			ImGui::ColorEdit3("Specular: ", &specular.data.x);
			ImGui::DragFloat3("Attenuation: ", &attenuation.x);
			ImGui::DragFloat3("Direction: ", &direction.x);
			ImGui::DragFloat("Cutoff: ", &cutoff);
			ImGui::DragFloat("Outer cutoff: ", &outterCutoff);

			Component::drawImGui();

			ImGui::TreePop();
		}
	}

	std::string Light::toString() const
	{
		return "COMP LIGHT " + std::to_string(position.w) + " " +
							   Utils::vecToStringParsing(ambient.data) +
							   Utils::vecToStringParsing(diffuse.data) +
							   Utils::vecToStringParsing(specular.data) +
							   Utils::vecToStringParsing(attenuation) +
							   std::to_string(cutoff) + " " +
							   Utils::vecToStringParsing(direction) +
							   std::to_string(outterCutoff) + " " +
							   std::to_string(enable) + " " + std::to_string(shadow == nullptr);
	}

	void Light::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<Light>();
		auto light = gameObject.getComponent<Light>();

		iss >> light->position.w;

		iss >> light->ambient.data.r;
		iss >> light->ambient.data.g;
		iss >> light->ambient.data.b;
		iss >> light->ambient.data.a;

		iss >> light->diffuse.data.r;
		iss >> light->diffuse.data.g;
		iss >> light->diffuse.data.b;
		iss >> light->diffuse.data.a;

		iss >> light->specular.data.r;
		iss >> light->specular.data.g;
		iss >> light->specular.data.b;
		iss >> light->specular.data.a;

		iss >> light->attenuation.c;
		iss >> light->attenuation.l;
		iss >> light->attenuation.q;

		iss >> light->cutoff;

		iss >> light->direction.x;
		iss >> light->direction.y;
		iss >> light->direction.z;

		iss >> light->outterCutoff;

		iss >> light->enable;

		bool hasShadow = false;
		iss >> hasShadow;
		light->setShadows(hasShadow);
	}
}