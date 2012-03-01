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
#ifndef PZ_sfImageResource_h__
#define PZ_sfImageResource_h__

#include <ProtoZed/Resource.h>

#include <SFML/Graphics/Image.hpp>

namespace PZ
{
	class sfImageResource : public Resource
	{
	public:
		sfImageResource()
		{}
		~sfImageResource()
		{}

		const sf::Image &GetImage() const
		{
			return image;
		}

	private:
		virtual bool load(const PZ::DataChunk &data)
		{
			return image.LoadFromMemory(data.GetData(), data.GetSize());
		}
		virtual bool unload()
		{
			return image.Create(0,0);
		}

		sf::Image image;
	};
}

#endif // PZ_sfImageResource_h__
