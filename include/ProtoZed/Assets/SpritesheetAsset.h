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
#ifndef PZ_SpritesheetAsset_h__
#define PZ_SpritesheetAsset_h__

#include <ProtoZed/Asset.h>
#include <ProtoZed/Vector2.h>
#include <ProtoZed/HashString.h>

#include <unordered_map>

namespace PZ
{
	class SpritesheetAsset : public Asset
	{
	public:
		SpritesheetAsset();
		~SpritesheetAsset();

		struct Animation
		{
			std::string name;
			unsigned int from, to;
			float time;
			bool loop;
		};

		std::string image;
		Vector2i size;
		typedef std::unordered_map<HashString, Animation> AnimationMap;
		AnimationMap animations;

	private:
		virtual bool load(const DataChunk &data);
		virtual bool unload();
	};
}

#endif // PZ_SpritesheetAsset_h__