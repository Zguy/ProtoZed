/*
Copyright (c) 2012 Johannes Häggqvist

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <ProtoZed/Log.h>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>

namespace PZ
{
	typedef std::map<std::string, Log*> LogMap;

	class LogManager::Impl
	{
	public:
		LogMap logs;
	};

	LogManager::LogManager() : p(new Impl)
	{
	}
	LogManager::~LogManager()
	{
		for (LogMap::iterator it = p->logs.begin(); it != p->logs.end(); ++it)
		{
			delete (*it).second;
			(*it).second = nullptr;
		}
		p->logs.clear();

		delete p;
	}

	void LogManager::Open(const std::string &name, bool consoleOutput, bool fileOutput, bool timestamp)
	{
		if (p->logs.find(name) == p->logs.end())
		{
			p->logs.insert(std::make_pair(name, new Log(name+".log", consoleOutput, fileOutput, timestamp)));
		}
	}
	void LogManager::Close(const std::string &name)
	{
		LogMap::iterator it = p->logs.find(name);
		if (it != p->logs.end())
		{
			delete (*it).second;
			p->logs.erase(it);
		}
	}

	Log &LogManager::Get(const std::string &name)
	{
		Open(name);

		return *p->logs.at(name);
	}


	typedef std::vector<Log::Listener*> ListenerList;

	class Log::Impl
	{
	public:
		std::string typeToString(Log::Type type)
		{
			switch (type)
			{
			case Log::LT_INFO    : return "Info";    break;
			case Log::LT_ERROR   : return "Error";   break;
			case Log::LT_WARNING : return "Warning"; break;
			case Log::LT_DEBUG   : return "Debug";   break;
			default              : return "";        break;
			}
		}

		std::string currentTime()
		{
			time_t currTime = std::time(nullptr);
			tm *timeStruct = localtime(&currTime);
			char *timeStr = new char[10];
			std::strftime(timeStr, 10, "%H:%M:%S", timeStruct);
			std::string str = timeStr;
			delete[] timeStr;
			return str;
		}
		std::string currentDateTime()
		{
			time_t currTime = std::time(nullptr);
			tm *timeStruct = localtime(&currTime);
			char *timeStr = new char[20];
			std::strftime(timeStr, 20, "%d/%m/%Y %H:%M:%S", timeStruct);
			std::string str = timeStr;
			delete[] timeStr;
			return str;
		}

		void logIntroLine()
		{
			if (fileOutput)
			{
				std::string introLine = "Log opened";
				if (timestamp)
					introLine += " "+currentDateTime();
				introLine += "\n--------";

				logFile << introLine << std::endl;
			}
		}

		void emitMessageLogged(Log::Type type, const std::string &message)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->MessageLogged(type, message);
			}
		}

		std::fstream logFile;

		bool consoleOutput;
		bool fileOutput;
		bool timestamp;

		ListenerList listeners;
	};

	Log::Log(const std::string &file, bool consoleOutput, bool fileOutput, bool timestamp) : p(new Impl)
	{
		p->consoleOutput = consoleOutput;
		p->fileOutput    = fileOutput;
		p->timestamp     = timestamp;

		if (p->fileOutput)
		{
			p->logFile.open(file.c_str(), std::ios::out | std::ios::trunc);

			p->logIntroLine();
		}
	}
	Log::~Log()
	{
		p->logFile.close();

		delete p;
	}

	void Log::SetConsoleOutput(bool enable)
	{
		p->consoleOutput = enable;
	}
	bool Log::IsConsoleOutputEnabled() const
	{
		return p->consoleOutput;
	}

	bool Log::IsFileOutputEnabled() const
	{
		return p->fileOutput;
	}

	void Log::SetTimestamp(bool enable)
	{
		p->timestamp = enable;
	}
	bool Log::IsTimestampEnabled() const
	{
		return p->timestamp;
	}

	void Log::Message(Type type, const std::string &message)
	{
		std::string typeStr = p->typeToString(type);
		std::string timeStr = p->currentTime();

		std::string line = typeStr;
		if (p->timestamp)
			line += " ("+timeStr+")";
		line += ": " + message;

		if (p->fileOutput)
			p->logFile << line << std::endl;
		if (p->consoleOutput)
			std::cout << line << std::endl;

		p->emitMessageLogged(type, message);
	}

	void Log::RegisterListener(Listener *listener)
	{
		if (listener != nullptr)
		{
			p->listeners.push_back(listener);
		}
	}
	void Log::UnregisterListener(Listener *listener)
	{
		for (ListenerList::iterator it = p->listeners.begin(); it != p->listeners.end(); ++it)
		{
			if ((*it) == listener)
			{
				p->listeners.erase(it);
				break;
			}
		}
	}
}