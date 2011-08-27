/*
	Copyright 2010-2011 Johannes Häggqvist

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
#include <ProtoZed/Logging/LogManager.h>

#include <map>

namespace PZ
{
	typedef std::map<std::string, Log*> LogMap;

	class LogManagerImpl
	{
	public:
		LogMap logs;
	};

	LogManager::LogManager() : p(new LogManagerImpl)
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
}