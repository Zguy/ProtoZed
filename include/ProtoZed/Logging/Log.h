/*
	Copyright 2010-2011 Johannes H�ggqvist

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

	private:
		Log(const std::string &file);
		~Log();

	public:
		void Message(Type type, const std::string &message);

		inline void Info(const std::string &message) { Message(LT_INFO, message); }
		inline void Error(const std::string &message) { Message(LT_ERROR, message); }
		inline void Warning(const std::string &message) { Message(LT_WARNING, message); }
		inline void Debug(const std::string &message) { Message(LT_DEBUG, message); }

	private:
		LogImpl *p;
	};
}

#endif // Log_h__