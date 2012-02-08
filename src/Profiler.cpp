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
#include <ProtoZed/Profiler.h>

#include <ProtoZed/Clock.h>

#include <stack>
#include <map>
#include <fstream>
#include <limits>

namespace PZ
{
	struct ProfilerEntry
	{
		ProfilerEntry(const std::string &path, const std::string &name) : path(path), name(name)
		{}

		std::string path;
		std::string name;
		Clock clock;
	};
	struct ProfilerResult
	{
		ProfilerResult() : worst(0.f), best(std::numeric_limits<float>::max()), total(0.f), count(0)
		{}

		void AddTime(float time)
		{
			total += time;

			++count;

			if (time > worst)
				worst = time;
			if (time < best)
				best = time;
		}

		float GetAverage() const
		{
			return total / static_cast<float>(count);
		}

		float worst;
		float best;
		float total;
		int count;
	};

	typedef std::stack<ProfilerEntry> RunningStack;
	typedef std::map<std::string, ProfilerResult> EntryMap;

	class Profiler::Impl
	{
	public:
		RunningStack runningEntries;
		EntryMap entries;

		std::string currentPath;
	};

	Profiler::Profiler() : p(new Impl)
	{
	}
	Profiler::~Profiler()
	{
		delete p;
	}

#ifdef PROFILER
	void Profiler::Begin(const std::string &name)
	{
		p->runningEntries.push(ProfilerEntry(p->currentPath, name));

		p->currentPath += name + '/';
	}
	void Profiler::End()
	{
		ProfilerEntry &entry = p->runningEntries.top();
		p->entries[entry.path+entry.name].AddTime(entry.clock.GetElapsedTime()*1000.f);

		p->currentPath.erase(p->currentPath.rfind(entry.name+'/'));

		p->runningEntries.pop();
	}

	void Profiler::WriteLog(const std::string &filename) const
	{
		//TODO: Maybe output as json instead?
		std::fstream log(filename+".log", std::ios::out | std::ios::trunc);

		for (EntryMap::const_iterator it = p->entries.begin(); it != p->entries.end(); ++it)
		{
			log << "avg: " << (*it).second.GetAverage() << " ms\tbest: " << (*it).second.best << " ms\tworst: " << (*it).second.worst << " ms\t" << (*it).second.count << " times\t" << (*it).first << "\n";
		}

		log.close();
	}
#endif

	Profile::Profile(const std::string &name) : profiler(Profiler::GetSingleton())
	{
		profiler.Begin(name);
	}
	Profile::~Profile()
	{
		profiler.End();
	}
}