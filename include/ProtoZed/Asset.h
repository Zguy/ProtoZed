/*
Copyright (c) 2012 Johannes H�ggqvist

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
#ifndef PZ_Asset_h__
#define PZ_Asset_h__

#include <ProtoZed/NonCopyable.h>
#include <ProtoZed/DataChunk.h>
#include <ProtoZed/Path.h>

namespace PZ
{
	class Asset : public NonCopyable
	{
		friend class AssetManager;

	public:
		Asset()
		{}
		virtual ~Asset()
		{}

		const Path &GetFilename() const
		{
			return filename;
		}

	private:
		virtual bool load(const DataChunk &data) = 0;
		virtual bool unload() = 0;

		Path filename;
	};
}

#endif // PZ_Asset_h__