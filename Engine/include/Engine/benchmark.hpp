#pragma once
#include <chrono>
#include <atomic>
#include <vector>
#include <string>

#include "singleton.hpp"

namespace Core::Engine
{
	class Benchmark final : public Singleton<Benchmark>
	{
		friend class Singleton<Benchmark>;

	private:
		float averageTimeMono = 0.f;
		float averageTimeMulti = 0.f;
		float benchRatio = 0.f;

		std::chrono::steady_clock::time_point startLoad;
		std::chrono::steady_clock::time_point endLoad;

		long sceneTrianglesCount = 0;
		long sceneTexturesCount  = 0;
		long sceneModelsCount    = 0;
		long sceneObjectsCount	 = 0; // Objects can have the same model

		std::atomic<bool> onBenchmark {false};

	public:
		std::vector<float> loadingTimesMono;
		std::vector<float> loadingTimesMulti;

		int benchmarkReloadCount = 0;
		int benchmarkMaxReloadCount = 0;
		bool timerEnd = false;

		static Benchmark* getInstance();

		static void addTrianglesCount(long count);
		static void addTexturesCount(long count);
		static void addModelsCount(long count);
		static void addObjectsCount(long count);

		long getTrianglesCount();
		long getTexturesCount();
		long getModelsCount();
		long getObjectsCount();

		float getAverageMono();
		float getAverageMulti();
		float getRatio();

		bool isActive();
		void setActive(bool active);
		void reset();
		void clear();

		static void startTimer();
		static void endTimer(bool isMultithreaded);

		void writeInFile(const std::string& path);
	};

}