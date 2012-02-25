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

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace
{
	boost::filesystem::path normalizePath(boost::filesystem::path &path)
	{
		std::vector<boost::filesystem::path> pathParts;

		for (boost::filesystem::path::iterator it = path.begin(); it != path.end(); ++it)
		{
			std::string part = (*it).generic_string();

			if (part == ".")
			{
				continue;
			}
			if (part == "..")
			{
				pathParts.pop_back();
				continue;
			}

			pathParts.push_back(*it);
		}

		boost::filesystem::path final;
		for (std::vector<boost::filesystem::path>::iterator it = pathParts.begin(); it != pathParts.end(); ++it)
		{
			final /= (*it);
		}

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

	bool FileSystemArchive::Open(const std::string &filename)
	{
		boost::filesystem::path path(filename);
		if (boost::filesystem::is_directory(path))
		{
			basePath = path.generic_string();

			return true;
		}

		return false;
	}
	bool FileSystemArchive::Close()
	{
		basePath = "";

		return true;
	}
	bool FileSystemArchive::IsOpen() const
	{
		return !basePath.empty();
	}

	bool FileSystemArchive::Has(const std::string &filename) const
	{
		if (IsOpen())
		{
			boost::filesystem::path base(basePath);
			boost::filesystem::path path(filename);
			boost::filesystem::path fullPath = base / path;
			return boost::filesystem::is_regular_file(fullPath);
		}
		else
		{
			return false;
		}
	}
	DataChunk FileSystemArchive::Get(const std::string &filename) const
	{
		if (IsOpen())
		{
			boost::filesystem::path base(basePath);
			boost::filesystem::path path(filename);
			boost::filesystem::path fullPath = base / path;
			if (boost::filesystem::is_regular_file(fullPath))
			{
				boost::filesystem::fstream file(fullPath, std::ios::in | std::ios::binary);
				if (file.is_open())
				{
					file.seekg(0, std::ios::end);
					unsigned int size = static_cast<unsigned int>(file.tellg());
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

			boost::filesystem::path base(basePath);
			for (boost::filesystem::recursive_directory_iterator it = boost::filesystem::recursive_directory_iterator(base); it != boost::filesystem::recursive_directory_iterator(); ++it)
			{
				boost::filesystem::path filePath = (*it).path();

				if (boost::filesystem::is_regular_file(filePath))
				{
					list.push_back(normalizePath(filePath).generic_string());
				}
			}
		}
	}
}
