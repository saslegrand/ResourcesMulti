#pragma once

#include "component.hpp"
#include "shader.hpp"
#include "cube_map.hpp"
#include "mesh.hpp"

namespace LowRenderer
{
	class SkyBox : public Engine::Component
	{
	private:
		std::shared_ptr<Resources::CubeMap> cubeMap;
		std::shared_ptr<Resources::Mesh> cubeMesh;
		std::shared_ptr<Resources::ShaderProgram> m_shaderProgram = nullptr;

		std::vector<std::string> skyPaths;

	public:
		SkyBox(Engine::GameObject& gameObject, const std::vector<std::string>& paths, bool isMulti);
		SkyBox(Engine::GameObject& gameObject, std::shared_ptr<SkyBox> ptr);
		~SkyBox();

		void draw() const;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss, bool isMulti);
	};
}