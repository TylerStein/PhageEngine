#pragma once
#include <fstream>
#include <string>

class LogManager
{
public:
	static LogManager* instance();
	void trace(std::string msg);
	void info(std::string msg);
	void warning(std::string msg);
	void error(std::string msg);

	std::string defFileName;
	enum LogType { LOG_NONE, LOG_ERROR, LOG_WARN, LOG_TRACE, LOG_INFO };

private:
	LogManager();
	~LogManager();

	static LogManager* _instance;
	LogType logType;
	std::ofstream *logStream;

public:
	void openFile(std::string &filename);

	void log(LogType type, std::string msg);

	bool writeThreshold(LogType type);

	void setType(LogType type)
	{
		logType = type;
	}

	LogType getLogType()
	{
		return logType;
	}

	void wipeFile();
};
