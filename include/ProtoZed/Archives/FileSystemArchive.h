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
#ifndef PZ_FileSystemArchive_h__
#define PZ_FileSystemArchive_h__

#include <ProtoZed/Archive.h>

namespace PZ
{
	class FileSystemArchive : public Archive
	{
	public:
		FileSystemArchive();
		~FileSystemArchive();

		virtual bool Open(const std::string &filename);
		virtual bool Close();
		virtual bool IsOpen();

		virtual bool Has(const std::string &filename) const;
		virtual DataChunk Get(const std::string &filename) const;

		virtual void GetAllFiles(FileList &list) const;

	private:
		std::string basePath;
	};
}

#endif // PZ_FileSystemArchive_h__
