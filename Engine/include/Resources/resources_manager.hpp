#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <memory>

#include "singleton.hpp"
#include "benchmark.hpp"

#include "character.hpp"
#include "cube_map.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "recipe.hpp"
#include "model.hpp"
#include "scene.hpp"
#include "mesh.hpp"

namespace Resources
{
	class ResourcesManager final : public Singleton<ResourcesManager>
	{
		friend Singleton<ResourcesManager>;

	private:
		bool initialized = false;

		ResourcesManager();
		~ResourcesManager();

		Core::Engine::Benchmark* bench = nullptr;

		std::vector<std::shared_ptr<Resource>> defaultResources;

		// Resources containers
		std::unordered_map<std::string, std::shared_ptr<Mesh>>			meshes;
		std::unordered_map<std::string, std::shared_ptr<Material>>		materials;
		std::unordered_map<std::string, std::shared_ptr<Texture>>		textures;
		std::unordered_map<std::string, std::shared_ptr<Shader>>		shaders;
		std::unordered_map<std::string, std::shared_ptr<CubeMap>>		cubeMaps;
		std::unordered_map<std::string, std::shared_ptr<Font>>			fonts;

		std::unordered_map<std::string, std::shared_ptr<LowRenderer::Model>>  models;

		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderPrograms;
		std::unordered_map<std::string, std::shared_ptr<Recipe>> recipes;

		// TODO : delete
		std::map<int, std::string> map;


		// Atomic flags (safe lists)
		std::atomic_flag lockTextures = ATOMIC_FLAG_INIT;
		std::atomic_flag lockMeshes = ATOMIC_FLAG_INIT;
		std::atomic_flag lockMaterials = ATOMIC_FLAG_INIT;
		std::atomic_flag lockCubeMaps = ATOMIC_FLAG_INIT;

		void setDefaultResources();
		void loadDefaults();

		template <class C>
		void clearMap(std::unordered_map<std::string, std::shared_ptr<C>>& map)
		{
			for (auto it = map.begin(); it != map.end();)
			{
				if (it->second.use_count() <= 1)
					it = map.erase(it);
				else
					it++;
			}
		}

		void loadObjMono(const std::string& filePath, bool onBenchmark);
		void loadObjMulti(const std::string& filePath);

		void loadMaterialsFromMtlMono(const std::string& dirPath, const std::string& mtlName);
		void loadMaterialsFromMtlMulti(const std::string& dirPath, const std::string& mtlName);

	public:
		static void init();

		static void clearTextures();
		static void clearResources();

		static std::shared_ptr<LowRenderer::Model> loadObj(const std::string& filePath, bool isMulti = true);

		static std::shared_ptr<Material> loadMaterial(const std::string& matName);
		static std::shared_ptr<Texture> loadTexture(const std::string& texName, bool isMultiThreaded = true);
		static std::shared_ptr<Texture> loadTexture(const std::string& name, int width, int height, float* data);
		static std::shared_ptr<Shader> loadShader(const std::string& shaderPath);
		static std::shared_ptr<CubeMap> loadCubeMap(const std::vector<std::string>& cubeMapPaths, bool isMultiThreaded = true);
		static std::shared_ptr<Font> loadFont(const std::string& fontPath);
		static std::shared_ptr<ShaderProgram> loadShaderProgram(const std::string& programName, const std::string& vertPath = "", const std::string& fragPath = "", const std::string& geomPath = "");
		static std::shared_ptr<Recipe> loadRecipe(const std::string& recipePath);

		static std::shared_ptr<Mesh> getMeshByName(const std::string& meshName);
		static std::shared_ptr<Material> getMatByName(const std::string& matName);
	};
}