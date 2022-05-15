#pragma once
#include <functional>
#include <atomic>
#include <chrono>
#include <queue>

#include "singleton.hpp"
#include "benchmark.hpp"

#include "scene.hpp"
#include "game_object.hpp"

#define MAX_REQUESTS_PER_FRAME 3

namespace Core::Engine
{
	class Graph final : public Singleton<Graph>
	{
		friend class Singleton<Graph>;

	private:
		Graph();
		~Graph();

		void drawBenchmark();
		void benchmark();

		bool showDemoWindowImGui = false;
		bool isMultithreaded = true;
		bool hasToReload = false;

		std::string sceneName;

		Resources::Scene curScene;

		std::queue<std::function<void()>> requests;
		std::atomic_flag requestFlag = ATOMIC_FLAG_INIT;

		Benchmark* bench = nullptr;

	public:
		void loadScene(const std::string& scenePath, bool onTask = false);
		void reload(const std::string& _sceneName = "");

		//static Graph* getInstance() { return instance(); }

		template <typename Request>
		static void addRequest(Request request)
		{
			Graph* graph = instance();

			if (graph == nullptr)
				return;

			while (graph->requestFlag.test_and_set());

			graph->requests.push(std::function<void()>(request));

			graph->requestFlag.clear();
		}

		static void saveCurrentScene();

		static void init();

		static Resources::Scene& getCurScene();

		static void draw();
		static void update();
		static void drawImGui();
		static void fixedUpdate();
		static void clean();
		static void checkRequests();
		static void deleteGameObject(const std::string& goName);

		static ::Engine::GameObject* findGameObjectWithName(const std::string& gameObjectName);

		static void addToDestroyQueue(::Engine::Object* obj);

		static bool getCursorState();
		static void setCursorState(bool state);

		static ::Engine::GameObject& instantiate(const std::string& GOname);
		static ::Engine::GameObject& instantiate(const std::string& GOname, const std::string& recipePath);
	};
}