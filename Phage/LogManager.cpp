#include "LogManager.h"

//Declare _instance as not null but something that satisfies the if statement below
LogManager* LogManager::_instance = 0;

LogManager* LogManager::instance()
{
	//Check if instance of Log Manager is in memory
	//Create a new instance if none exists
	if (_instance == 0)
	{
		_instance = new LogManager;
	}

	return _instance;
}

LogManager::LogManager()
{
	logStream = NULL;
	logType = LOG_TRACE;
	defFileName = "log.txt";
}

LogManager::~LogManager()
{
	wipeFile();
}

void LogManager::openFile(std::string &filename)
{
	wipeFile();
	logStream = new std::ofstream(filename.c_str());
}

void LogManager::log(LogType type, std::string msg)
{
	if (writeThreshold(type))
	{
		if (logStream == NULL)
		{
			openFile(defFileName);
		}
		(*logStream) << msg << "\n";
		logStream->flush();
	}
}

bool LogManager::writeThreshold(LogType type)
{
	if (type <= logType && type > LOG_NONE)
	{
		return true;
	}
	return false;
}

void LogManager::trace(std::string msg)
{
	log(LOG_TRACE, msg);
}
void LogManager::info(std::string msg)
{
	log(LOG_INFO, msg);
}
void LogManager::warning(std::string msg)
{
	log(LOG_WARN, msg);
}
void LogManager::error(std::string msg)
{
	log(LOG_ERROR, msg);
}

void LogManager::wipeFile()
{
	if (logStream != NULL)
	{
		logStream->close();
		delete logStream;
		logStream = NULL;
	}
}