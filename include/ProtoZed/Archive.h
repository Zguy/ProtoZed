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
#ifndef PZ_Archive_h__
#define PZ_Archive_h__

#include <ProtoZed/DataChunk.h>
#include <ProtoZed/Path.h>

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>

namespace PZ
{
	typedef std::string ArchiveType;

	typedef std::vector<Path> FileList;

	class Archive : public boost::noncopyable
	{
	public:
		Archive()
		{}
		virtual ~Archive()
		{}

		virtual bool Open(const Path &filename) = 0;
		virtual bool Close() = 0;
		virtual bool IsOpen() const = 0;

		virtual bool Has(const Path &filename) const = 0;
		virtual DataChunk Get(const Path &filename) const = 0;

		virtual void GetAllFiles(FileList &list) const = 0;
	};
}

#endif // PZ_Archive_h__
