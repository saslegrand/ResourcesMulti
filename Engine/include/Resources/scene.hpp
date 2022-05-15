#pragma once

#include <unordered_map>

#include "game_object.hpp"
#include "model_renderer.hpp"

namespace Resources
{
	class Scene
	{
	private:
		std::string curGoName = "";

		void setGameObjectParent(const std::string& goName, const std::string& goChildName);

		std::vector<Engine::Object*> objectsToDestroy;

	public:
		bool cursorState = true;

		std::string filePath;
		std::unordered_map<std::string, Engine::GameObject> gameObjects;

		Scene() = default;
		Scene(const std::string& path);
		~Scene();

		void load(const std::string& filePath, bool isMultiThreaded = true);
		void save(const std::string& path);
		void draw() const;
		void update();
		void fixedUpdate();

		void clear();

		void addModel(const char* buffer, bool isMulti);

		void cleanObjects();
		void checkLoadedResources();

		Engine::GameObject* findGameObjectWithName(const std::string& gameObjectName);

		void deleteGameObject(const std::string& goName);

		void drawHierarchy();
		void drawInspector();

		void addToDestroyQueue(Engine::Object* objToDestroy);

		std::string getUniqueGOName(const std::string& gameObjectName);
		Engine::GameObject& instantiate(const std::string& gameObjectName = "GameObject");
		Engine::GameObject& instantiate(const std::string& gameObjectName, const std::string& recipePath);
	};
}