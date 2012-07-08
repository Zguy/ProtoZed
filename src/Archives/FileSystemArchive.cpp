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
#include <ProtoZed/Archives/FileSystemArchive.h>

#include <ProtoZed/Platform.h>

#include <boost/algorithm/string.hpp>

#include <fstream>

namespace
{
	std::string stripBase(const std::string &path, const std::string &base)
	{
		std::string final = path;

		boost::replace_head(final, base.size()+1, "");

		return final;
	}
}

namespace PZ
{
	FileSystemArchive::FileSystemArchive() : basePath("")
	{
	}
	FileSystemArchive::~FileSystemArchive()
	{
	}

	bool FileSystemArchive::Open(const Path &filename)
	{
		if (filename.GetAttributes().type == Path::T_DIRECTORY)
		{
			basePath = filename;

			return true;
		}

		return false;
	}
	bool FileSystemArchive::Close()
	{
		basePath.Clear();

		return true;
	}
	bool FileSystemArchive::IsOpen() const
	{
		return !basePath.IsEmpty();
	}

	bool FileSystemArchive::Has(const Path &filename) const
	{
		if (IsOpen())
		{
			return ((basePath + filename).GetAttributes().type == Path::T_FILE);
		}
		else
		{
			return false;
		}
	}
	DataChunk FileSystemArchive::Get(const Path &filename) const
	{
		if (IsOpen())
		{
			Path fullPath = basePath + filename;

			if (fullPath.GetAttributes().type == Path::T_FILE)
			{
				std::fstream file(fullPath.ToString().c_str(), std::ios::in | std::ios::binary);
				if (file.is_open())
				{
					file.seekg(0, std::ios::end);
					size_t size = static_cast<size_t>(file.tellg());
					file.seekg(0, std::ios::beg);

					char *data = new char[size];
					file.read(data, size);

					file.close();

					return DataChunk(data, size);
				}
			}
		}

		return DataChunk();
	}

	void FileSystemArchive::GetAllFiles(FileList &list) const
	{
		if (IsOpen())
		{
			list.clear();

			std::vector<std::string> tlist;
			Platform::GetFileList(tlist, basePath.ToString());

			for (std::vector<std::string>::const_iterator it = tlist.cbegin(); it != tlist.cend(); ++it)
			{
				list.push_back(stripBase(*it, basePath.ToString()));
			}
		}
	}
}
