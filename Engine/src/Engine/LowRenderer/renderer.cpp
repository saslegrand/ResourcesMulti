#include "renderer.hpp"

#include "render_manager.hpp"
#include "resources_manager.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Renderer::Renderer(Engine::GameObject& gameObject, const std::shared_ptr<Renderer>& childPtr, const std::string& shaderProgramName, bool link)
		: Component(gameObject, childPtr), m_shaderProgram(Resources::ResourcesManager::loadShaderProgram(shaderProgramName))
	{
		m_transform = requireComponent<Physics::Transform>();
	}

	std::shared_ptr<Resources::ShaderProgram> Renderer::getProgram()
	{
		return m_shaderProgram;
	}
}