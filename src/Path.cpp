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
#include <ProtoZed/Platform.h>

#include <boost/algorithm/string.hpp>

#include <vector>

namespace PZ
{
	Path::Path()
	{
	}
	Path::Path(const Path &other) : path(other.path)
	{
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

	Path &Path::operator+=(const Path &other)
	{
		path += "/" + other.path;

		return *this;
	}

	Path::Attributes Path::GetAttributes() const
	{
		return Platform::GetAttributes(path);
	}

	bool Path::Exists() const
	{
		return (GetAttributes().type != T_NONEXISTANT);
	}

	void Path::normalize()
	{
		if (!path.empty())
		{
			//Profile profile("NormalizePath");

			std::string dirtyPath = path;

			std::vector<std::string> pathParts;
			boost::split(pathParts, dirtyPath, boost::is_any_of("/\\"), boost::token_compress_on);

			std::string cleanPath;

			for (std::vector<std::string>::const_iterator it = pathParts.cbegin(); it != pathParts.cend(); ++it)
			{
				if ((*it).empty() || (*it) == "." || (*it) == "..")
					continue;

				if (!cleanPath.empty())
					cleanPath += "/";

				cleanPath += (*it);
			}

			if (cleanPath.empty())
			{
				// If cleanPath is empty, we want the current directory
				cleanPath = ".";
			}

			path = cleanPath;
		}
	}
}
