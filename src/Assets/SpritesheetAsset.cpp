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
#include <ProtoZed/Assets/SpritesheetAsset.h>
#include <ProtoZed/Jzon.h>
#include <ProtoZed/Log.h>

namespace PZ
{
	SpritesheetAsset::SpritesheetAsset()
	{
	}
	SpritesheetAsset::~SpritesheetAsset()
	{
	}

	bool SpritesheetAsset::load(const DataChunk &data)
	{
		std::string str(data.GetData(), data.GetSize());

		Jzon::Object root;
		Jzon::Parser parser(root, str);
		if (!parser.Parse())
		{
			Log::Error("ProtoZed", "Failed to parse spritesheet: "+parser.GetError());
			return false;
		}

		image = root.Get("image").ToString();
		size.x = root.Get("width").ToInt();
		size.y = root.Get("height").ToInt();

		if (root.Has("animations"))
		{
			Jzon::Array &animationArray = root.Get("animations").AsArray();
			for (Jzon::Array::iterator it = animationArray.begin(); it != animationArray.end(); ++it)
			{
				Jzon::Object &animObj = (*it).AsObject();
				Animation anim;
				anim.name = animObj.Get("name").ToString();
				anim.from = animObj.Get("start").ToInt();
				anim.to   = animObj.Get("end").ToInt();
				anim.time = animObj.Get("time").ToFloat();
				anim.loop = animObj.Get("loop").ToBool();

				animations.insert(std::make_pair(HashString(anim.name), anim));
			}
		}

		return true;
	}
	bool SpritesheetAsset::unload()
	{
		image.clear();
		size .x = size.y = 0;
		animations.clear();

		return true;
	}
}