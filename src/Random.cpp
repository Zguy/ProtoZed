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

#include <boost/random.hpp>

#include <ctime>

namespace
{
	static boost::random::mt19937 _gen;
	static unsigned int _seed = 0;
}

namespace PZ
{
	unsigned int Random::GetSeed()
	{
		return _seed;
	}
	void Random::SetSeed(unsigned int seed)
	{
		_gen.seed(seed);
		_seed = seed;
	}
	void Random::SetSeedFromTime()
	{
		SetSeed(static_cast<unsigned int>(time(nullptr)));
	}

	int Random::NextInt(int min, int max)
	{
		boost::random::uniform_int_distribution<> dist(min, max);
		return dist(_gen);
	}
	float Random::NextFloat(float min, float max)
	{
		boost::random::uniform_real_distribution<float> dist(min, max);
		return dist(_gen);
	}
}
