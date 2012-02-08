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
#include <ctime>
#include <iostream>
#include <map>

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
		}
		p->logs.clear();

		delete p;
	}

	void LogManager::OpenLog(const std::string &name)
	{
		if (p->logs.find(name) == p->logs.end())
		{
			p->logs.insert(std::make_pair(name, new Log(name+".log")));
		}
	}
	void LogManager::CloseLog(const std::string &name)
	{
		LogMap::iterator it = p->logs.find(name);
		if (it != p->logs.end())
		{
			delete (*it).second;
			p->logs.erase(it);
		}
	}

	Log &LogManager::GetLog(const std::string &name)
	{
		OpenLog(name);

		return *p->logs.at(name);
	}


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

		//TODO: Move these into helper functions
		std::string currentTime()
		{
			time_t currTime = std::time(nullptr);
			tm *timeStruct = new tm;
			localtime_s(timeStruct, &currTime);
			char *timeStr = new char[10];
			std::strftime(timeStr, 10, "%H:%M:%S", timeStruct);
			std::string str = timeStr;
			delete timeStruct;
			delete[] timeStr;
			return str;
		}
		std::string currentDateTime()
		{
			time_t currTime = std::time(nullptr);
			tm *timeStruct = new tm;
			localtime_s(timeStruct, &currTime);
			char *timeStr = new char[20];
			std::strftime(timeStr, 20, "%d/%m/%Y %H:%M:%S", timeStruct);
			std::string str = timeStr;
			delete timeStruct;
			delete[] timeStr;
			return str;
		}

		void logIntroLine()
		{
			std::string dateTimeStr = currentDateTime();
			std::string introLine = "Log opened "+dateTimeStr+"\n--------";
			logFile << introLine << std::endl;
		}

		std::fstream logFile;
	};

	Log::Log(const std::string &file) : p(new Impl)
	{
		p->logFile.open(file.c_str(), std::ios::out | std::ios::trunc);

		p->logIntroLine();
	}
	Log::~Log()
	{
		p->logFile.close();

		delete p;
	}

	void Log::Message(Type type, const std::string &message)
	{
		std::string typeStr = p->typeToString(type);
		std::string timeStr = p->currentTime();

		std::string line = typeStr + " ("+timeStr+"): " + message;
		p->logFile << line << std::endl;
		std::cout << line << std::endl;
	}
}