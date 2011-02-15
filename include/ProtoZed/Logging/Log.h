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
#ifndef Log_h__
#define Log_h__

#include <string>

namespace PZ
{
	class LogImpl;

	class Log
	{
		friend class LogManager;

	public:
		enum Type
		{
			LT_INFO,
			LT_ERROR,
			LT_WARNING,
			LT_DEBUG
		};
		enum Level
		{
			LVL_LOW    = 1, // Least important
			LVL_MEDIUM = 2,
			LVL_HIGH   = 3  // Most important
		};

	private:
		Log(const std::string &file, Log::Level level);
		~Log();

	public:
		void Message(Type type, Level level, const std::string &message);

		inline void Info(Level level, const std::string &message) { Message(LT_INFO, level, message); }
		inline void Error(Level level, const std::string &message) { Message(LT_ERROR, level, message); }
		inline void Warning(Level level, const std::string &message) { Message(LT_WARNING, level, message); }
		inline void Debug(Level level, const std::string &message) { Message(LT_DEBUG, level, message); }

	private:
		void setLogLevel(Log::Level level);

		LogImpl *p;
	};
}

#endif // Log_h__