#include "resources_manager.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

#include "define.hpp"
#include "debug.hpp"

#include "thread_pool.hpp"
#include "graph.hpp"
#include "maths.hpp"
#include "utils.hpp"

namespace Resources
{
	#pragma region ____UTILS_FUNCTIONS____

	void addData(std::vector<Core::Maths::vec3>& dataVector, std::istringstream& iss)
	{
		//// Get a 3D Vector data form string stream
		Core::Maths::vec3 data = { 0.f };

		iss >> data.x;
		iss >> data.y;
		iss >> data.z;

		dataVector.push_back(data);
	}

	LowRenderer::Color getColor(std::istringstream& iss)
	{
		// Get a Color data form string stream
		LowRenderer::Color color = { 0.f };

		iss >> color.data.r;
		iss >> color.data.g;
		iss >> color.data.b;

		return color;
	}

	// Use to know if needed to triangulate faces
	int getNumFace(const std::string& line)
	{
		int numFace = 0;

		// Check how many spaces the line has
		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == ' ')
				numFace++;
		}

		return numFace;
	}

	// Give the number of vertices of a face from an .obj
	int numVerticesFace(const std::string& line)
	{
		int numV = 0;

		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == '/')
				numV++;
		}

		if (numV > 6)
			return 4;
		else
			return 3;
	}

	// Allow to know if face is of type v//vn or v/uv/vn
	int getFaceType(const std::string& line)
	{
		for (size_t i = 3; i < line.length(); i++)
		{
			if (line[i - 1] == '/' && line[i] == '/')
				return 1;
		}

		return 0;
	}

	void addIndices(std::vector<unsigned int>& indices, std::istringstream& iss, const std::string& line)
	{
		unsigned int indicesVertices[4];
		unsigned int indicesUV[4];
		unsigned int indicesNormals[4];

		// Number of vertices of the face 
		int numV = numVerticesFace(line);

		// Data type of the face
		int type = getFaceType(line);

		for (int i = 0; i < numV; i++)
		{
			// Face of type v/uv/vn
			if (type == 0)
			{
				iss >> indicesVertices[i];
				iss.ignore();
				iss >> indicesUV[i];
				iss.ignore();
				iss >> indicesNormals[i];
				iss.ignore();
			}
			// Face of type v//vn
			else
			{
				iss >> indicesVertices[i];
				iss.ignore();
				iss.ignore();
				iss >> indicesNormals[i];

				indicesUV[i] = 1;
			}

			int indexV2Strip = i;
			// Strip faces (triangulation)
			if (i > 2)
			{
				indices.push_back(indicesVertices[0] - 1);
				indices.push_back(indicesUV[0] - 1);
				indices.push_back(indicesNormals[0] - 1);
				indices.push_back(indicesVertices[i - 1] - 1);
				indices.push_back(indicesUV[i - 1] - 1);
				indices.push_back(indicesNormals[i - 1] - 1);
			}

			// Add vertex indices to the mesh indices
			indices.push_back(indicesVertices[i] - 1);
			indices.push_back(indicesUV[i] - 1);
			indices.push_back(indicesNormals[i] - 1);
		}
	}

#pragma endregion

	#pragma region ____MANAGER_FUNCTIONS____

	ResourcesManager::ResourcesManager()
	{
		Core::Debug::Log::info("Creating the Resources Manager");
	}

	ResourcesManager::~ResourcesManager()
	{
		Core::Debug::Log::info("Destroying the Resources Manager");

		//defaultResources.clear();
		//clearResources();
	}

	void ResourcesManager::loadDefaults()
	{
		loadObj("resources/obj/cube.obj", false);
		defaultResources.push_back(getMeshByName("cube"));
		loadObj("resources/obj/sphere.obj", false);
		defaultResources.push_back(getMeshByName("sphere"));
		loadObj("resources/obj/plane.obj", false);
		defaultResources.push_back(getMeshByName("plane"));

		defaultResources.push_back(loadTexture("resources/UI/benchText.png", false));
		defaultResources.push_back(loadTexture("resources/UI/benchWheel.png", false));

		// Set default textures and materials
		setDefaultResources();
	}

	void ResourcesManager::setDefaultResources()
	{
		// White color
		float whiteBuffer[4] = { 1.f, 1.f, 1.f, 1.f };

		// Black color
		float blackBuffer[4] = { 0.f, 0.f, 0.f, 0.f };

		// Purple and black grid
		//float noDiffuseBuffer[16] = { 1.f, 0.f, 0.863f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.863f, 1.f };
		float noDiffuseBuffer[4] = { 0.8f, 0.8f, 0.8f, 0.8f };

		// Load the default textures
		std::shared_ptr<Texture> whiteTex = ResourcesManager::loadTexture("whiteTex", 1, 1, whiteBuffer);
		defaultResources.push_back(whiteTex);
		std::shared_ptr<Texture> blackTex = ResourcesManager::loadTexture("blackTex", 1, 1, blackBuffer);
		defaultResources.push_back(blackTex);
		std::shared_ptr<Texture> noDiffuseTex = ResourcesManager::loadTexture("noDiffuseTex", 1, 1, noDiffuseBuffer);
		defaultResources.push_back(noDiffuseTex);

		// Set the default textures
		Texture::defaultAlpha = whiteTex;
		Texture::defaultAmbient = whiteTex;
		Texture::defaultDiffuse = noDiffuseTex;
		Texture::defaultEmissive = blackTex;
		Texture::defaultSpecular = whiteTex;

		Material::defaultMaterial = ResourcesManager::loadMaterial("defaultMat");
		defaultResources.push_back(Material::defaultMaterial);
	}

	void ResourcesManager::init()
	{
		ResourcesManager* RM = instance();

		// Check if the ResourcesManager instance is already initialized
		if (RM->initialized)
		{
			Core::Debug::Log::error("The Resources Manager is already initialized");
			return;
		}

		RM->initialized = true;
		Core::Debug::Log::info("Resources Manager initialized");

		RM->bench = Core::Engine::Benchmark::getInstance();

		loadShaderProgram("shader", "resources/shaders/vertexShader.vert", "resources/shaders/fragmentShader.frag");
		loadShaderProgram("skyBox", "resources/shaders/skyBox.vert", "resources/shaders/skyBox.frag");
		loadShaderProgram("colliderShader", "resources/shaders/vertexCollider.vert", "resources/shaders/fragmentCollider.frag");
		loadShaderProgram("spriteShader", "resources/shaders/spriteVertex.vert", "resources/shaders/spriteFragment.frag");
		loadShaderProgram("depthShader", "resources/shaders/depthShader.vert", "resources/shaders/depthShader.frag");
		loadShaderProgram("depthCubeShader", "resources/shaders/depthCubeShader.vert", "resources/shaders/depthShader.frag", "resources/shaders/depthCubeShader.geom");

		// Set default textures and materials
		RM->loadDefaults();

	}

	void ResourcesManager::clearResources()
	{
		ResourcesManager* RM = instance();

		if (RM == nullptr)
			return;

		RM->clearMap(RM->models);
		RM->clearMap(RM->meshes);
		RM->clearMap(RM->materials);
		RM->clearMap(RM->cubeMaps);
		RM->clearMap(RM->textures);
	}

	std::shared_ptr<Shader> ResourcesManager::loadShader(const std::string& shaderPath)
	{
		ResourcesManager* RM = instance();
		
		const auto& shaderIt = RM->shaders.find(shaderPath);

		// Check if the Shader is already loaded
		if (shaderIt != RM->shaders.end())
		{
			return shaderIt->second;
		}

		return RM->shaders[shaderPath] = std::make_shared<Shader>(shaderPath);
	}

	std::shared_ptr<ShaderProgram> ResourcesManager::loadShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
	{
		ResourcesManager* RM = instance();

		const auto& programIt = RM->shaderPrograms.find(programName);

		// Check if the ShaderProgram is already loaded
		if (programIt != RM->shaderPrograms.end())
			return programIt->second;

		return RM->shaderPrograms[programName] = std::make_shared<ShaderProgram>(programName, vertPath, fragPath, geomPath);
	}

	std::shared_ptr<Font> ResourcesManager::loadFont(const std::string& fontPath)
	{
		ResourcesManager* RM = instance();

		const auto& fontIt = RM->fonts.find(fontPath);

		// Check if the Texture is already loaded
		if (fontIt != RM->fonts.end())
		{
			return fontIt->second;
		}

		return RM->fonts[fontPath] = std::make_shared<Font>(Font(fontPath));
	}

	void ResourcesManager::clearTextures()
	{
		instance()->textures.clear();
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& texName, int width, int height, float* data)
	{
		ResourcesManager* RM = instance();

		std::string absPath = PROJECT_DIR + texName;

		while (RM->lockTextures.test_and_set());

		const auto& textureIt = RM->textures.find(absPath);

		// Check if the Texture is already loaded
		if (textureIt != RM->textures.end())
		{
			//Core::Debug::Log::info("Texture " + texName + " is already loaded");
			RM->lockTextures.clear();
			return textureIt->second;
		}

		// Get texture (avoid reading when returning)
		RM->textures[absPath] = std::make_shared<Texture>();
		auto tex = RM->textures[absPath];

		RM->lockTextures.clear();

		tex->set(absPath, width, height, data);

		return tex;
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& texName, bool isMultiThreaded)
	{
		ResourcesManager* RM = instance();

		std::string absPath = PROJECT_DIR + texName;

		while (RM->lockTextures.test_and_set());

		const auto& textureIt = RM->textures.find(absPath);

		// Check if the Texture is already loaded
		if (textureIt != RM->textures.end())
		{
			//Core::Debug::Log::info("Texture " + texName + " is already loaded");
			RM->lockTextures.clear();
			return textureIt->second;
		}

		// Get texture (avoid reading when returning)
		RM->textures[absPath] = std::make_shared<Texture>();
		auto tex = RM->textures[absPath];

		RM->lockTextures.clear();

		if (isMultiThreaded)
			ThreadPool::addTask(std::bind(&Texture::create, tex.get(), absPath, true));
		else
			tex->create(absPath, RM->bench->isActive());

		return tex;
	}

	std::shared_ptr<CubeMap> ResourcesManager::loadCubeMap(const std::vector<std::string>& cubeMapPaths, bool isMulti)
	{
		ResourcesManager* RM = instance();

		if (cubeMapPaths.size() == 0)
			return nullptr;

		while (RM->lockCubeMaps.test_and_set());

		std::string pathsDir = Utils::getDirectory(cubeMapPaths.back());

		const auto& cubeMapIt = RM->cubeMaps.find(pathsDir);

		// Check if the Texture is already loaded
		if (cubeMapIt != RM->cubeMaps.end())
		{
			RM->lockCubeMaps.clear();
			return cubeMapIt->second;
		}

		RM->cubeMaps[pathsDir] = std::make_shared<CubeMap>();

		auto cubeMap = RM->cubeMaps[pathsDir];

		RM->lockCubeMaps.clear();

		Core::Debug::Log::info("Start creating skybox at " + pathsDir);

		if (isMulti)
			ThreadPool::addTask(std::bind(&CubeMap::create, cubeMap.get(), cubeMapPaths, true));
		else
			cubeMap->create(cubeMapPaths, RM->bench->isActive());

		return cubeMap;
	}

	std::shared_ptr<Material> ResourcesManager::loadMaterial(const std::string& matName)
	{
		ResourcesManager* RM = instance();

		while (RM->lockMaterials.test_and_set());

		const auto& materialIt = RM->materials.find(matName);

		// Check if the Texture is already loaded
		if (materialIt != RM->materials.end())
		{
			// Get material (avoid reading when returning)
			auto mat = materialIt->second;
			RM->lockMaterials.clear();

			return mat;
		}

		// Get material (avoid reading when returning)
		auto mat = RM->materials[matName] = std::make_shared<Material>();
		
		RM->lockMaterials.clear();

		return mat;
	}

	std::shared_ptr<Recipe> ResourcesManager::loadRecipe(const std::string& recipePath)
	{
		ResourcesManager* RM = instance();

		const auto& recipeIt = RM->recipes.find(recipePath);

		// Check if the Material is already loaded
		if (recipeIt != RM->recipes.end())
			return recipeIt->second;

		return RM->recipes[recipePath] = std::make_shared<Recipe>(recipePath);
	}

	std::shared_ptr<LowRenderer::Model> ResourcesManager::loadObj(const std::string& filePath, bool isMulti)
	{
		ResourcesManager* RM = instance();

		// Check if the object is already loaded
		if (RM->models.find(filePath) != RM->models.end())
		{
			Core::Debug::Log::info("Model at " + filePath + " is already loaded");
			return RM->models[filePath];
		}

		Core::Engine::Benchmark::addModelsCount(1);

		// Create the model and set model ptr
		RM->models[filePath] = std::make_shared<LowRenderer::Model>(LowRenderer::Model(filePath));

		if (isMulti)
			ThreadPool::addTask(std::bind(&ResourcesManager::loadObjMulti, RM, filePath));
		else
			RM->loadObjMono(filePath, RM->bench->isActive());

		return RM->models[filePath];
	}

	void ResourcesManager::loadObjMulti(const std::string& filePath)
	{
		std::ifstream dataObj((PROJECT_DIR + filePath).c_str());

		// Check if the file exists
		if (!dataObj)
		{
			Core::Debug::Log::error("Unable to read the file : " + filePath);
			dataObj.close();
			return;
		}

		auto start = std::chrono::steady_clock::now();

		std::shared_ptr<LowRenderer::Model> model = models[filePath];

		Core::Debug::Log::info("Start loading obj " + filePath);

		std::vector<Core::Maths::vec3> vertices;
		std::vector<Core::Maths::vec3> texCoords;
		std::vector<Core::Maths::vec3> normals;
		std::vector<unsigned int> indices;
		std::vector<std::string> names;
		std::string dirPath = Utils::getDirectory(filePath);

		bool isFirstObject = true;
		Resources::Mesh mesh;

		std::string matName = "default";

		// Core::Debug::Log::info("Loading meshes");

		std::string line;
		while (std::getline(dataObj, line))
		{
			std::istringstream iss(line);
			std::string type;

			iss >> type;

			if (type == "#" || type == "" || type == "\n")
				continue;

			if (type == "o" || type == "g")
			{
				if (isFirstObject)
					isFirstObject = false;
				else
				{
					if (!indices.empty())
					{
						// Compute and add the mesh
						mesh.compute(vertices, texCoords, normals, indices);

						while (lockMeshes.test_and_set());

						// Add the mesh on RM
						meshes[mesh.name] = std::make_shared<Mesh>(mesh);
						// Link mesh and material to the model
						Core::Engine::Graph::addRequest(std::bind(&LowRenderer::Model::addSubModel, model.get(), meshes[mesh.name], loadMaterial(matName)));

						lockMeshes.clear();

						indices.clear();
					}

					mesh = Mesh();
					matName = "default";
				}

				iss >> mesh.name;
			}
			else if (type == "v")
				addData(vertices, iss);
			else if (type == "vt")
				addData(texCoords, iss);
			else if (type == "vn")
				addData(normals, iss);
			else if (type == "f")
				addIndices(indices, iss, line);
			else if (type == "usemtl")
				iss >> matName;
			else if (type == "mtllib")
			{
				std::string mtlName;
				iss >> mtlName;

				// Load mtl file
				ThreadPool::addTask(std::bind(&ResourcesManager::loadMaterialsFromMtlMulti, this, dirPath, mtlName));
			}
		}

		if (!indices.empty())
		{
			// Compute and add the mesh
			mesh.compute(vertices, texCoords, normals, indices);

			while (lockMeshes.test_and_set());

			// Add the mesh on RM
			meshes[mesh.name] = std::make_shared<Mesh>(mesh);
			// Link mesh and material to the model
			Core::Engine::Graph::addRequest(std::bind(&LowRenderer::Model::addSubModel, model.get(), meshes[mesh.name], loadMaterial(matName)));

			lockMeshes.clear();
		}
		
		dataObj.close();

		Core::Debug::Log::info("Finish loading obj meshes " + filePath);
		auto end = std::chrono::steady_clock::now();

		float time = (float)(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000.f;
		Core::Debug::Log::info("Obj " + filePath + " took " + std::to_string(time) + " sec to load");

	}

	// Load an obj with mtl (do triangulation)
	void ResourcesManager::loadObjMono(const std::string& filePath, bool onBenchmark)
	{
		std::ifstream dataObj((PROJECT_DIR + filePath).c_str());

		// Check if the file exists
		if (!dataObj)
		{
			Core::Debug::Log::error("Unable to read the file : " + filePath);
			dataObj.close();
			return;
		}

		auto start = std::chrono::steady_clock::now();

		std::shared_ptr<LowRenderer::Model> model = models[filePath];

		Core::Debug::Log::info("Start loading obj " + filePath);

		std::vector<Core::Maths::vec3> vertices;
		std::vector<Core::Maths::vec3> texCoords;
		std::vector<Core::Maths::vec3> normals;
		std::vector<unsigned int> indices;
		std::vector<std::string> names;
		std::string dirPath = Utils::getDirectory(filePath);

		bool isFirstObject = true;
		Resources::Mesh mesh;
		std::string matName;

		Core::Debug::Log::info("Loading meshes");

		std::string line;
		while (std::getline(dataObj, line))
		{
			std::istringstream iss(line);
			std::string type;

			iss >> type;

			if (type == "#" || type == "" || type == "\n")
				continue;


			if (type == "o" || type == "g")
			{
				if (isFirstObject)
					isFirstObject = false;
				else
				{
					if (!indices.empty())
					{
						// Compute and add the mesh
						mesh.compute(vertices, texCoords, normals, indices);

						// Add the mesh on RM
						meshes[mesh.name] = std::make_shared<Mesh>(mesh);

						if (onBenchmark)
						{
							model->m_subModels.push_back(LowRenderer::SubModel(meshes[mesh.name], loadMaterial(matName)));

							// Link mesh and material to the model
							Core::Engine::Graph::addRequest(std::bind(&Mesh::generateVAO, meshes[mesh.name].get()));
						}
						else
							model->addSubModel(meshes[mesh.name], loadMaterial(matName));
						
					}

					mesh = Mesh();
					indices.clear();
				}

				iss >> mesh.name;
			}
			else if (type == "v")
				addData(vertices, iss);
			else if (type == "vt")
				addData(texCoords, iss);
			else if (type == "vn")
				addData(normals, iss);
			else if (type == "f")
				addIndices(indices, iss, line);
			else if (type == "usemtl")
				iss >> matName;
			else if (type == "mtllib")
			{
				std::string mtlName;
				iss >> mtlName;

				// Load mtl file
				loadMaterialsFromMtlMono(dirPath, mtlName);
			}
		}

		if (!indices.empty())
		{
			// Compute and generate the mesh
			mesh.compute(vertices, texCoords, normals, indices);

			// Add the mesh on RM
			meshes[mesh.name] = std::make_shared<Mesh>(mesh);

			if (onBenchmark)
			{
				// Link mesh and material to the model
				model->m_subModels.push_back(LowRenderer::SubModel(meshes[mesh.name], loadMaterial(matName)));

				// Generate VAO
				Core::Engine::Graph::addRequest(std::bind(&Mesh::generateVAO, meshes[mesh.name].get()));
			}
			else
				model->addSubModel(meshes[mesh.name], loadMaterial(matName));
		}

		dataObj.close();

		Core::Debug::Log::info("Finish loading meshes " + filePath);

		auto end = std::chrono::steady_clock::now();

		float time = (float)(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000.f;
		Core::Debug::Log::info("Obj " + filePath + " took " + std::to_string(time) + " sec to load");

		return;
	}

	void ResourcesManager::loadMaterialsFromMtlMulti(const std::string & dirPath, const std::string & mtlName)
	{
		std::string filePath = dirPath + mtlName;

		// Check if the file exist
		std::ifstream dataMat((PROJECT_DIR + filePath).c_str());
		if (!dataMat)
		{
			Core::Debug::Log::error("Unable to read the file: " + filePath);
			dataMat.close();
			return;
		}

		std::shared_ptr<Material> mat;
		std::string line;
		std::string matName;
		bool isFirstMat = true;

		Core::Debug::Log::info("Loading materials at " + filePath);

		// Get all mesh materials
		while (std::getline(dataMat, line))
		{
			std::istringstream iss(line);
			std::string type;
			iss >> type;

			if (type == "#" || type == "" || type == "\n")
				continue;

			if (type == "newmtl")
			{
				if (isFirstMat)
					isFirstMat = false;
				else
				{
					//setMaterial(mat);
					mat->isDrawable = true;
				}

				iss >> matName;
				mat = loadMaterial(matName);

				continue;
			}
			else if (type == "Ns")
				iss >> mat->shininess;
			else if (type == "Ka")
				mat->ambient = getColor(iss);
			else if (type == "Kd")
				mat->diffuse = getColor(iss);
			else if (type == "Ks")
				mat->specular = getColor(iss);
			else if (type == "Ke")
				mat->emissive = getColor(iss);
			else if (type == "Ni")
				iss >> mat->opticalDensity;
			else if (type == "d")
				iss >> mat->transparency;
			else if (type == "illum")
			{
				iss >> mat->illumination;
				continue;
			}

			std::string texName;
			iss >> texName;

			// Load mesh textures
			if (type == "map_Ka")
				mat->ambientTex = loadTexture(dirPath + Utils::getFileNameFromPath(texName), true);
			else if (type == "map_Kd")
				mat->diffuseTex = loadTexture(dirPath + Utils::getFileNameFromPath(texName), true);
			else if (type == "map_Ks")
				mat->specularTex = loadTexture(dirPath + Utils::getFileNameFromPath(texName), true);
			else if (type == "map_Ke")
				mat->emissiveTex = loadTexture(dirPath + Utils::getFileNameFromPath(texName), true);
			else if (type == "map_d")
				mat->alphaTex = loadTexture(dirPath + Utils::getFileNameFromPath(texName), true);
		}

		mat->isDrawable = true;

		dataMat.close();
	}

	void ResourcesManager::loadMaterialsFromMtlMono(const std::string& dirPath, const std::string& mtlName)
	{
		std::string filePath = dirPath + mtlName;

		// Check if the file exist
		std::ifstream dataMat((PROJECT_DIR + filePath).c_str());
		if (!dataMat)
		{
			Core::Debug::Log::error("Unable to read the file: " + filePath);
			dataMat.close();
			return;
		}

		std::shared_ptr<Material> mat;
		std::string line;
		std::string matName;
		bool isFirstMat = true;

		Core::Debug::Log::info("Loading materials at " + filePath);

		// Get all mesh materials
		while (std::getline(dataMat, line))
		{
			std::istringstream iss(line);
			std::string type;
			iss >> type;

			if (type == "#" || type == "" || type == "\n")
				continue;

			if (type == "newmtl")
			{
				if (isFirstMat)
					isFirstMat = false;
				else
					mat->isDrawable = true;

				iss >> matName;
				mat = loadMaterial(matName);

				continue;
			}
			else if (type == "Ns")
				iss >> mat->shininess;
			else if (type == "Ka")
				mat->ambient = getColor(iss);
			else if (type == "Kd")
				mat->diffuse = getColor(iss);
			else if (type == "Ks")
				mat->specular = getColor(iss);
			else if (type == "Ke")
				mat->emissive = getColor(iss);
			else if (type == "Ni")
				iss >> mat->opticalDensity;
			else if (type == "d")
				iss >> mat->transparency;
			else if (type == "illum")
				iss >> mat->illumination;
			
			std::string texName;
			iss >> texName;

			// Load mesh textures
			if (type == "map_Ka")
				mat->ambientTex  = loadTexture(dirPath + Utils::getFileNameFromPath(texName), false);
			else if (type == "map_Kd")
				mat->diffuseTex  = loadTexture(dirPath + Utils::getFileNameFromPath(texName), false);
			else if (type == "map_Ks")
				mat->specularTex = loadTexture(dirPath + Utils::getFileNameFromPath(texName), false);
			else if (type == "map_Ke")
				mat->emissiveTex = loadTexture(dirPath + Utils::getFileNameFromPath(texName), false);
			else if (type == "map_d")
				mat->alphaTex    = loadTexture(dirPath + Utils::getFileNameFromPath(texName), false);
		}

		mat->isDrawable = true;

		dataMat.close();
	}

	// Return a mesh from its name (Thread safe)
	std::shared_ptr<Mesh> ResourcesManager::getMeshByName(const std::string& meshName)
	{
		ResourcesManager* RM = instance();

		while (RM->lockMeshes.test_and_set());

		auto meshIt = RM->meshes.find(meshName);

		// Check if the mesh exist
		if (meshIt == RM->meshes.end())
		{
			Core::Debug::Log::error("Can not find mesh named " + meshName);
			RM->lockMeshes.clear();

			return nullptr;
		}

		RM->lockMeshes.clear();

		return meshIt->second;
	}

	// Return a material from its name (Thread safe)
	std::shared_ptr<Material> ResourcesManager::getMatByName(const std::string& matName)
	{
		ResourcesManager* RM = instance();

		while (RM->lockMaterials.test_and_set());

		auto materialIt = RM->materials.find(matName);

		// Check if a material is link to the mesh name
		if (materialIt == RM->materials.end())
		{
			Core::Debug::Log::error("Can not find material at " + matName);
			RM->lockMaterials.clear();

			return nullptr;
		}

		RM->lockMaterials.clear();

		// Load and return the material
		return materialIt->second;
	}

#pragma endregion
}