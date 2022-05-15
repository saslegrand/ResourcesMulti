#include "debug.hpp"

#include <fstream>
#include <cassert>

namespace Core
{
	namespace Debug
	{
		Log::~Log()
		{
			Log::info("Destroying the Logs Manager");

			Log::saveToFile();
		}

		void Log::saveToFile()
		{
			Log* logManager = Log::instance();

			// Create a new log file
			std::ofstream currentFile("logs/log.txt");

			// Put all the logs in it
			currentFile << logManager->logs;

			// Clear the logs
			logManager->logs.clear();
		}

		void Log::out(const std::string& log, LogType logType)
		{
			// Get the current time as a char*
			std::time_t currentTime = std::time(0);	// Get current time

			char timeString[26];
			ctime_s(timeString, sizeof(timeString), &currentTime);
			timeString[24] = '\0';

			// Set the current log format
			std::string currentLog = "[" + std::string(timeString) + "] " + log + '\n';

			Log* logManager = Log::instance();

			while (logManager->flag.test_and_set());

			// Put the logs in the Log Manager 
			logManager->logs += currentLog;

			// Cout the current log
			std::cout << currentLog;

			logManager->flag.clear();
		}

		Assertion::Assertion()
		{
			Log::info("Creating the Assertions Manager");
		}

		Assertion::~Assertion()
		{
			Log::info("Destroying the Assertions Manager");
		}

		void Assertion::out(const std::exception& assertion)
		{
			// Throw assertion if an exception has been handled
			throw  assertion;
		}
	}
}