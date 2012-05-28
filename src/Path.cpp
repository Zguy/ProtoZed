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
#include <ProtoZed/Path.h>

#include <ProtoZed/Profiler.h>

#include <boost/filesystem/path.hpp>

#include <vector>

namespace PZ
{
	Path::Path()
	{
	}
	Path::Path(const Path &other)
	{
		path = other.path;
	}
	Path::Path(const std::string &path) : path(path)
	{
		normalize();
	}
	Path::Path(const char *path) : path(path)
	{
		normalize();
	}
	Path::~Path()
	{
	}

	Path &Path::operator=(const Path &other)
	{
		if (this != &other)
		{
			path = other.path;
		}

		return *this;
	}
	Path &Path::operator=(const std::string &path)
	{
		this->path = path;
		normalize();

		return *this;
	}
	Path &Path::operator=(const char *path)
	{
		this->path = path;
		normalize();

		return *this;
	}

	void Path::normalize()
	{
		if (!path.empty())
		{
			//Profile profile("NormalizePath");

			boost::filesystem::path dirtyPath = path;

			std::vector<boost::filesystem::path> pathParts;

			for (boost::filesystem::path::const_iterator it = dirtyPath.begin(); it != dirtyPath.end(); ++it)
			{
				std::string part = (*it).generic_string();

				if (part == ".")
				{
					continue;
				}
				if (part == "..")
				{
					if (!pathParts.empty() && pathParts.back() != "..")
					{
						pathParts.pop_back();
						continue;
					}
				}

				pathParts.push_back(*it);
			}

			if (pathParts.empty())
			{
				// If pathParts is empty, we want the current directory
				pathParts.push_back(".");
			}

			boost::filesystem::path cleanPath;
			for (std::vector<boost::filesystem::path>::iterator it = pathParts.begin(); it != pathParts.end(); ++it)
			{
				cleanPath /= (*it);
			}

			path = cleanPath.generic_string();
		}
	}
}
