#include "benchmark.hpp"

#include <fstream>

#include "utils.hpp"

namespace Core::Engine
{
	void Benchmark::addTrianglesCount(long count)
	{
		instance()->sceneTrianglesCount += count;
	}

	void Benchmark::addTexturesCount(long count)
	{
		instance()->sceneTexturesCount += count;
	}

	void Benchmark::addModelsCount(long count)
	{
		instance()->sceneModelsCount += count;
	}

	void Benchmark::addObjectsCount(long count)
	{
		instance()->sceneObjectsCount += count;
	}

	long Benchmark::getTrianglesCount()
	{
		return sceneTrianglesCount;
	}

	long Benchmark::getTexturesCount()
	{
		return sceneTexturesCount;
	}

	long Benchmark::getModelsCount()
	{
		return sceneModelsCount;
	}

	long Benchmark::getObjectsCount()
	{
		return sceneObjectsCount;
	}

	float Benchmark::getAverageMono()
	{
		return averageTimeMono;
	}

	float Benchmark::getAverageMulti()
	{
		return averageTimeMulti;
	}

	float Benchmark::getRatio()
	{
		return benchRatio;
	}

	void Benchmark::reset()
	{
		sceneTrianglesCount = 0;
		sceneModelsCount	= 0;
		sceneTexturesCount	= 0;
		sceneObjectsCount	= 0;
	}

	void Benchmark::clear()
	{
		loadingTimesMono.clear();
		loadingTimesMulti.clear();
	}
	
	bool Benchmark::isActive()
	{
		return onBenchmark.load();
	}

	void Benchmark::setActive(bool active)
	{
		onBenchmark.store(active);
	}

	void Benchmark::startTimer()
	{
		instance()->startLoad = std::chrono::steady_clock::now();
	}

	void getAverageTime(std::vector<float>& times, float& averageTime, float curTime)
	{
		times.push_back(curTime);

		float loadingTimesSum = 0.f;
		for (float time : times)
			loadingTimesSum += time;

		averageTime = loadingTimesSum / (float)(times.size());
	}

	void Benchmark::endTimer(bool isMultithreaded)
	{
		Benchmark* BM = instance();
		// Set end time
		BM->endLoad = std::chrono::steady_clock::now();

		// Calculate average time
		{
			float loadingTime = (float)(std::chrono::duration_cast<std::chrono::milliseconds>(BM->endLoad - BM->startLoad).count()) / 1000.f;

			if (isMultithreaded)
				getAverageTime(BM->loadingTimesMulti, BM->averageTimeMulti, loadingTime);
			else
				getAverageTime(BM->loadingTimesMono, BM->averageTimeMono, loadingTime);

			if (BM->averageTimeMulti != 0.f)
				BM->benchRatio = BM->averageTimeMono / BM->averageTimeMulti;
		}
	}

	Benchmark* Benchmark::getInstance()
	{
		return instance();
	}

	void Benchmark::writeInFile(const std::string& path)
	{
		std::ofstream flux(path);

		if (!flux) return;

		flux << "______________ Benchmark loading scenes logs ______________\n\n\n";

		flux << "Scene objects count : " + std::to_string(sceneObjectsCount);
		flux << " (for " + std::to_string(sceneModelsCount) + " models loaded)\n";
		flux << "Scene triangles count : " + std::to_string(sceneTrianglesCount) + "\n";
		flux << "Scene textures count : " + std::to_string(sceneTexturesCount) + "\n";

		flux << "___________________________________________________________\n\n";

		flux << "Multithread loading times :\n\n";

		for (size_t i = 0; i < loadingTimesMulti.size(); i++)
			flux << "Time " + std::to_string(i + 1) + " -> " + Utils::getTimeStrFromSeconds(loadingTimesMulti[i]) + '\n';

		flux << "\nAverage time -> " + Utils::getTimeStrFromSeconds(averageTimeMulti) + "\n";

		flux << "___________________________________________________________\n\n";

		flux << "Monothread loading times :\n\n";

		for (size_t i = 0; i < loadingTimesMono.size(); i++)
			flux << "Time " + std::to_string(i + 1) + " -> " + Utils::getTimeStrFromSeconds(loadingTimesMono[i]) + '\n';

		flux << "\nAverage time -> " + Utils::getTimeStrFromSeconds(averageTimeMono) + "\n";

		flux << "___________________________________________________________\n\n";

		if (benchRatio == 0.f) return;
		flux << "Ratio -> ";

		if (benchRatio >= 1.f)
			flux << "Multithread is " + std::to_string(benchRatio) + " faster than monothread";
		else
			flux << "Monothread is " + std::to_string(1.f / benchRatio) + " faster than multithread";

		flux.close();
	}
}