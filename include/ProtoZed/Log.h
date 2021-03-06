/*
Copyright (c) 2013 Johannes H�ggqvist

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
#ifndef PZ_Log_h__
#define PZ_Log_h__

#include <ProtoZed/Singleton.h>
#include <ProtoZed/EventHandler.h>

#include <boost/noncopyable.hpp>

#include <string>

namespace PZ
{
	class Log;

	class LogList : public Singleton<LogList>
	{
	public:
		LogList();
		~LogList();

		void Open(const std::string &name, bool consoleOutput = true, bool fileOutput = true, bool timestamp = true);
		void Close(const std::string &name);

		Log &Get(const std::string &name);

	private:
		class Impl;
		Impl *p;
	};

	class Log : boost::noncopyable, public EventHandler
	{
		friend class LogList;

	public:
		enum Type
		{
			LT_INFO,
			LT_ERROR,
			LT_WARNING,
			LT_DEBUG
		};

	private:
		Log(const std::string &file, bool consoleOutput = true, bool fileOutput = true, bool timestamp = true);
		~Log();

	public:
		void SetConsoleOutput(bool enable);
		bool IsConsoleOutputEnabled() const;

		bool IsFileOutputEnabled() const;

		void SetTimestamp(bool enable);
		bool IsTimestampEnabled() const;

		void Message(Type type, const std::string &message);

		inline void Info(const std::string &message) { Message(LT_INFO, message); }
		inline void Error(const std::string &message) { Message(LT_ERROR, message); }
		inline void Warning(const std::string &message) { Message(LT_WARNING, message); }
		inline void Debug(const std::string &message) { Message(LT_DEBUG, message); }

		static inline void Message(const std::string &name, Type type, const std::string &message) { LogList::GetSingleton().Get(name).Message(type, message); }

		static inline void Info(const std::string &name, const std::string &message) { Message(name, LT_INFO, message); }
		static inline void Error(const std::string &name, const std::string &message) { Message(name, LT_ERROR, message); }
		static inline void Warning(const std::string &name, const std::string &message) { Message(name, LT_WARNING, message); }
		static inline void Debug(const std::string &name, const std::string &message) { Message(name, LT_DEBUG, message); }

	private:
		class Impl;
		Impl *p;
	};
}

#endif // PZ_Log_h__
