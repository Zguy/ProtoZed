/*
	Copyright 2010 Johannes Häggqvist

	This file is part of ProtoZed.

	ProtoZed is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ProtoZed is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with ProtoZed.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <ProtoZed/Logging/Log.h>

#include <fstream>
#include <ctime>
#include <iostream>

namespace PZ
{
	class LogImpl
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
			time_t currTime = std::time(NULL);
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
			time_t currTime = std::time(NULL);
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
		Log::Level logLevel;
	};

	Log::Log(const std::string &file, Log::Level level) : p(new LogImpl)
	{
		p->logFile.open(file.c_str(), std::ios::out | std::ios::trunc);
		p->logLevel = level;

		p->logIntroLine();
	}
	Log::~Log()
	{
		p->logFile.close();

		delete p;
	}

	void Log::Message(Type type, Level level, const std::string &message)
	{
		if (level >= p->logLevel)
		{
			std::string typeStr = p->typeToString(type);
			std::string timeStr = p->currentTime();

			std::string line = typeStr + " ("+timeStr+"): " + message;
			p->logFile << line << std::endl;
			std::cout << line << std::endl;
		}
	}

	void Log::setLogLevel(Log::Level level)
	{
		p->logLevel = level;
	}
}