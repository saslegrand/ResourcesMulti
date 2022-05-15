#pragma once

#include "singleton.hpp"

#include <chrono>
#include <ctime>
#include <atomic>

#include <iostream>

namespace Core
{
	namespace Debug
	{
		enum class LogType
		{
			ERROR,
			ASSERTION,
			WARNING,
			INFO,
			EXCEPTION
		};

		class Log final : public Singleton<Log>
		{
			friend class Singleton<Log>;

		private:
			std::string logs;

			std::atomic_flag flag = ATOMIC_FLAG_INIT;

			~Log();

			static void saveToFile();

			// Output the casted log
			static void out(const std::string& log, LogType logType);

		public:
			template <typename T>
			static void exception(const T& log)
			{
				out(std::string("EXCEPTION: ") + log, LogType::EXCEPTION);
			}

			template <typename T>
			static void warning(const T& log)
			{
				out(std::string("Warning: ") + log, LogType::WARNING);
			}

			template <typename T>
			static void assertion(const T& log)
			{
				out(std::string("ASSERTION: ") + log, LogType::ASSERTION);
			}

			template <typename T>
			static void error(const T& log)
			{
				out(std::string("ERROR: ") + log, LogType::ERROR);
			}

			template <typename T>
			static void info(const T& log)
			{
				//while (instance()->flag.test_and_set());

				out(std::string("Info: ") + log, LogType::INFO);

				//instance()->flag.clear();
			}
		};

		class Assertion final : public Singleton<Assertion>
		{
			friend class Singleton<Assertion>;

		private:
			Assertion();
			~Assertion();

		public:
			// Get assertion
			static void out(const std::exception& assertion);

			// Get any supported type and check is the condition return true
			template <typename T>
			static void out(bool condition, const T& assertion)
			{
				// If the condition does not return true, return an error
				if (!condition)
					throw std::string(assertion);
			}
		};
	};
}