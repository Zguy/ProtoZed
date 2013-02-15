/*
Copyright (c) 2013 Johannes Häggqvist

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
#include <ProtoZed/Random.h>

#include <ProtoZed/Log.h>

#include <boost/random.hpp>

#include <ctime>
#include <map>

namespace PZ
{
	typedef std::map<std::string, Random*> RandomMap;

	class RandomManager::Impl
	{
	public:
		RandomMap randomMap;
	};

	Random *RandomManager::global = nullptr;

	RandomManager::RandomManager() : p(new Impl)
	{
		if (global == nullptr)
		{
			global = new Random;
		}
	}
	RandomManager::~RandomManager()
	{
		for (RandomMap::iterator it = p->randomMap.begin(); it != p->randomMap.end(); ++it)
		{
			delete (*it).second;
			(*it).second = nullptr;
		}
		p->randomMap.clear();

		if (global != nullptr)
		{
			delete global;
			global = nullptr;
		}

		delete p;
	}

	bool RandomManager::Add(const std::string &name)
	{
		return Add(name, static_cast<unsigned int>(std::time(nullptr)));
	}
	bool RandomManager::Add(const std::string &name, unsigned int seed)
	{
		Random *random = new Random(seed);
		if (!p->randomMap.insert(std::make_pair(name, new Random(seed))).second)
		{
			delete random;
			return false;
		}

		return true;
	}
	bool RandomManager::Remove(const std::string &name)
	{
		RandomMap::iterator it = p->randomMap.find(name);
		if (it != p->randomMap.end())
		{
			delete (*it).second;
			p->randomMap.erase(it);
			return true;
		}

		return false;
	}

	const Random &RandomManager::Get(const std::string &name) const
	{
		RandomMap::iterator it = p->randomMap.find(name);
		if (it != p->randomMap.end())
		{
			return *(*it).second;
		}

		Log::Warning("ProtoZed", "Random \""+name+"\" doesn't exist, returning global");
		return GetGlobal();
	}

	
	class Random::Impl
	{
	public:
		boost::random::mt19937 gen;
	};

	Random::Random() : p(new Impl)
	{
		p->gen.seed(static_cast<unsigned int>(time(nullptr)));
	}
	Random::Random(unsigned int seed) : p(new Impl)
	{
		p->gen.seed(seed);
	}
	Random::~Random()
	{
		delete p;
	}

	int Random::NextInt(int min, int max) const
	{
		boost::random::uniform_int_distribution<> dist(min, max);
		return dist(p->gen);
	}
	float Random::NextFloat(float min, float max) const
	{
		boost::random::uniform_real_distribution<float> dist(min, max);
		return dist(p->gen);
	}
}
