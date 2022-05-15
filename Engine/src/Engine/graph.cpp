#include "graph.hpp"

#include <fstream>

#include "imgui.h"

#include "resources_manager.hpp"
#include "physic_manager.hpp"
#include "engine_master.hpp"
#include "thread_pool.hpp"
#include "application.hpp"
#include "debug.hpp"
#include "time.hpp"

#include "utils.hpp"

namespace Core::Engine
{
	Graph::Graph()
	{
		Core::Debug::Log::info("Creating the Graph");
	}

	Graph::~Graph()
	{
		Core::Debug::Log::info("Destroying the Graph");

		bench->writeInFile("logs/benchmark.txt");

		Benchmark::kill();
	}

	void Graph::loadScene(const std::string& scenePath, bool onTask)
	{
		curScene.clear();

		if (onTask)
			ThreadPool::addTask(std::bind(&Resources::Scene::load, &curScene, scenePath, isMultithreaded));
		else
			curScene.load(scenePath, isMultithreaded);

		Core::TimeManager::resetTime();
	}

	void Graph::saveCurrentScene()
	{
		instance()->curScene.save(instance()->sceneName);
	}

	void Graph::init()
	{
		Graph* graph = Graph::instance();

		graph->bench = Benchmark::getInstance();

		graph->bench->reset();

		//Resources::ResourcesManager::init();

		graph->sceneName = "resources/scenes/scene.scn";
		graph->loadScene(graph->sceneName);
	}

	Resources::Scene& Graph::getCurScene()
	{
		return instance()->curScene;
	}

	void Graph::draw()
	{
		Graph* graph = instance();

		// Draw the scene
		graph->curScene.draw();
	}

	void Graph::update()
	{
		Graph* graph = instance();

		if (graph->bench->isActive())
			return;

		// Update the scene
		graph->curScene.update();

		// Update rigidbodies and colliders
		Physics::PhysicManager::update();
	}

	void Graph::fixedUpdate()
	{
		// Fixed update the scene
		instance()->curScene.fixedUpdate();
	}

	void Graph::clean()
	{
		instance()->curScene.cleanObjects();
	}

	void Graph::benchmark()
	{
		if (hasToReload)
		{
			Engine::EngineMaster::toggleEditMode();
			bench->timerEnd = false;
			hasToReload = false;
			reload();
		}

		if (!ThreadPool::running() && requests.empty())
		{
			if (!bench->timerEnd)
			{
				bench->endTimer(isMultithreaded);
				bench->timerEnd = true;
			}

			if (!bench->isActive()) return;

			if (bench->benchmarkReloadCount == 0)
			{
				if (isMultithreaded)
				{
					isMultithreaded = false;
					bench->benchmarkReloadCount = bench->benchmarkMaxReloadCount;
				}
				else
				{
					bench->setActive(false);
					EngineMaster::lockEditMode(false);
					EngineMaster::toggleEditMode();
					return;
				}
			}

			EngineMaster::lockEditMode(true);
			Core::Application::setCursor(true);
			bench->benchmarkReloadCount--;
			bench->timerEnd = false;
			reload();
		}
	}

	void Graph::checkRequests()
	{
		Graph* graph = instance();

		graph->benchmark();

		// Lock requests vector
		while (graph->requestFlag.test_and_set());

		for (int i = 0; i < MAX_REQUESTS_PER_FRAME; i++)
		{
			if (graph->requests.size() == 0)
				break;

			auto request = graph->requests.front();
			request();
			graph->requests.pop();
		}

		// Unlock the vector
		graph->requestFlag.clear();
	}

	void Graph::deleteGameObject(const std::string& goName)
	{
		instance()->curScene.deleteGameObject(goName);
	}

	void getFpsCount(int& fps)
	{
		static double curTime = ImGui::GetTime();
		static double prevTime = 0.0;
		static double delaySum = 0.0;
		static int frameCount = 0;

		prevTime = curTime;
		curTime = ImGui::GetTime();
		delaySum += curTime - prevTime;

		frameCount++;

		if (frameCount == 10)
		{
			fps = (int)(10.0 / delaySum);
			frameCount = 0;
			delaySum = 0.0;
		}
	}

	void Graph::reload(const std::string& _sceneName)
	{
		while (!requests.empty())
			requests.pop();

		bench->reset();

		std::string path = _sceneName == "" ? sceneName : _sceneName;

		loadScene(path, !isMultithreaded && bench->isActive());
	}

	void Graph::drawBenchmark()
	{
		static char bufferPathBench[512];

		if (!ThreadPool::running())
		{
			ImGui::Text("Benchmark : Reload n time in multi and mono");
			ImGui::InputInt("Reload count", &bench->benchmarkMaxReloadCount);

			if (ImGui::Button("Launch benchmark"))
			{
				bench->benchmarkReloadCount = bench->benchmarkMaxReloadCount;
				bench->setActive(true);
				isMultithreaded = true;
				bench->clear();
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("Save benchmark (logs/benchmark/... .txt), eg. bench.txt");

			ImGui::PushID(1);
			ImGui::InputText("", bufferPathBench, 512);
			ImGui::PopID();

			if (ImGui::Button("Save"))
			{
				std::string buffer{ bufferPathBench };
				std::string path = "logs/benchmark/" + buffer;
				bench->writeInFile(path);
			}

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
		}

		Utils::imGuiTextFromString("Objects count : " + std::to_string(bench->getObjectsCount())
			+ " (for " + std::to_string(bench->getModelsCount()) + " models loaded)");
		Utils::imGuiTextFromString("Triangles count : " + std::to_string(bench->getTrianglesCount()));
		Utils::imGuiTextFromString("Textures count : " + std::to_string(bench->getTexturesCount()));

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("___________________________");
		ImGui::Text("MultiThread loading times : ");

		ImGui::Spacing();

		for (size_t i = 0; i < bench->loadingTimesMulti.size(); i++)
			Utils::imGuiTextFromString("Time " + std::to_string(i + 1) + " -> " + Utils::getTimeStrFromSeconds(bench->loadingTimesMulti[i]));

		Utils::imGuiTextFromString("\nAverage time : " + Utils::getTimeStrFromSeconds(bench->getAverageMulti()));

		ImGui::Text("___________________________");
		ImGui::Text("Monothread loading times : ");

		ImGui::Spacing();

		for (size_t i = 0; i < bench->loadingTimesMono.size(); i++)
			Utils::imGuiTextFromString("Time " + std::to_string(i + 1) + " -> " + Utils::getTimeStrFromSeconds(bench->loadingTimesMono[i]));

		Utils::imGuiTextFromString("\nAverage time : " + Utils::getTimeStrFromSeconds(bench->getAverageMono()));

		float ratio = bench->getRatio();
		if (ratio == 0.f) return;

		ImGui::Text("___________________________");

		ImGui::Text("\nBased on average times");
		if (ratio >= 1.f)
			Utils::imGuiTextFromString("\nMultithread is " + std::to_string(ratio) + " faster than monothread");
		else
			Utils::imGuiTextFromString("\nMonothread is " + std::to_string(1.f / ratio) + " faster than multithread");
	}

	void Graph::drawImGui()
	{
		Graph* graph = instance();

		static int fps = 0;
		static char loadBuffer[256];
		static char sceneLoadBuffer[256];
		static char sceneCreateBuffer[256];
		static bool isMulti = true;

		if (ImGui::Begin("Graph"))
		{
			if (ImGui::Button("Show demo window"))
				graph->showDemoWindowImGui = !graph->showDemoWindowImGui;

			if (graph->showDemoWindowImGui)
				ImGui::ShowDemoWindow();

			ImGui::Spacing();

			getFpsCount(fps);
			std::string str = "FPS : " + std::to_string(fps);
			const char* cStr = str.c_str();
			ImGui::Text(cStr);

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Scene"))
			{
				const char* curSceneName = graph->sceneName.c_str();
				ImGui::Text("Current scene name : "); ImGui::SameLine();
				ImGui::Text(curSceneName);

				ImGui::Spacing();
				ImGui::Spacing();

				if (!ThreadPool::running())
				{
					ImGui::Checkbox("MultiThread", &graph->isMultithreaded);

					ImGui::Spacing();

					if (ImGui::BeginTabBar("##Tabs", 0))
					{
						if (ImGui::BeginTabItem("Load"))
						{
							ImGui::Text("Load save scene (resources/scenes/... .scn) eg. sceneSave.scn");

							ImGui::PushID(0);
							ImGui::InputText("", sceneLoadBuffer, 256);
							ImGui::PopID();

							if (ImGui::Button("Load scene"))
							{
								std::string strBuf = sceneLoadBuffer;

								if (strBuf != "")
									graph->sceneName = "resources/scenes/" + std::string(sceneLoadBuffer);

								graph->hasToReload = true;
							}

							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();

							if (ImGui::Button("Load template scene"))
							{
								std::string strBuf = sceneLoadBuffer;
								graph->sceneName = "resources/scenes/scene.scn";
								graph->hasToReload = true;
							}

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Reload"))
						{
							if (ImGui::Button("Reload current scene"))
								graph->hasToReload = true;

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Save"))
						{
							if (ImGui::Button("Save current scene"))
								graph->saveCurrentScene();

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Create"))
						{
							ImGui::Text("Create new scene from basic scene (resources/scenes/scene.scn)");
							ImGui::Spacing();

							ImGui::Text("Save scene name, eg. save.scn");
							ImGui::PushID(3);
							ImGui::InputText("", sceneCreateBuffer, 256);
							ImGui::PopID();

							if (ImGui::Button("Create scene"))
							{
								std::string strBuf = sceneCreateBuffer;

								if (strBuf != "")
								{
									graph->sceneName = "resources/scenes/" + std::string(sceneCreateBuffer);
									graph->saveCurrentScene();
									graph->hasToReload = true;
								}
							}

							ImGui::EndTabItem();
						}

						ImGui::EndTabBar();
					}
				}
				else
					ImGui::Text("Can't reload/save while threads running");
			}

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Benchmark"))
				graph->drawBenchmark();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Load model"))
			{
				ImGui::Text("Give obj path (from project dir)");
				ImGui::InputText("Obj path ", loadBuffer, 256);
				ImGui::Checkbox("Is multithreaded", &isMulti);
				if (ImGui::Button("Load (as \"NewModel\" gameObject)"))
					graph->curScene.addModel(loadBuffer, isMulti);
			}

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Hierarchy"))
				graph->curScene.drawHierarchy();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Separator();
		}
		ImGui::End();

		if (ImGui::Begin("Inspector"))
			graph->curScene.drawInspector();
		ImGui::End();
	}

	::Engine::GameObject* Graph::findGameObjectWithName(const std::string& gameObjectName)
	{
		return instance()->curScene.findGameObjectWithName(gameObjectName);
	}

	void Graph::addToDestroyQueue(::Engine::Object* obj)
	{
		instance()->curScene.addToDestroyQueue(obj);
	}

	bool Graph::getCursorState()
	{
		return instance()->curScene.cursorState;
	}

	void Graph::setCursorState(bool state)
	{
		instance()->curScene.cursorState = state;

		Core::Application::setCursor(state);
	}

	::Engine::GameObject& Graph::instantiate(const std::string& GOname)
	{
		return instance()->curScene.instantiate(GOname);
	}

	::Engine::GameObject& Graph::instantiate(const std::string& GOname, const std::string& recipePath)
	{
		return instance()->curScene.instantiate(GOname, recipePath);
	}
}