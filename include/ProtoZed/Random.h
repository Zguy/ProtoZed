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
#ifndef PZ_Random_h__
#define PZ_Random_h__

#include <boost/noncopyable.hpp>

#include <string>

namespace PZ
{
	class Random;

	class RandomManager : public boost::noncopyable
	{
	public:
		RandomManager();
		~RandomManager();

		bool Add(const std::string &name);
		bool Add(const std::string &name, unsigned int seed);
		bool Remove(const std::string &name);

		const Random &Get(const std::string &name) const;

		static const Random &GetGlobal()
		{
			return *global;
		}

	private:
		class Impl;
		Impl *p;

		static Random *global;
	};

	class Random : public boost::noncopyable
	{
	public:
		Random();
		Random(unsigned int seed);
		~Random();

		int NextInt(int min, int max) const;
		float NextFloat(float min, float max) const;

		static inline int GlobalNextInt(int min, int max) { return RandomManager::GetGlobal().NextInt(min, max); }
		static inline float GlobalNextFloat(float min, float max) { return RandomManager::GetGlobal().NextFloat(min, max); }

	private:
		class Impl;
		Impl *p;
	};
}

#endif // PZ_Random_h__
