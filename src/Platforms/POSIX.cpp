/*
Copyright (c) 2013 Johannes Häggqvist

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
#include <ProtoZed/Platform.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>

namespace PZ
{
	namespace Platform
	{
		double GetHighResolutionClock()
		{
			timeval time;
			gettimeofday(&time, 0);
			return (time.tv_sec + (time.tv_usec * 0.000001));
		}

		Path::Attributes GetAttributes(const std::string &file)
		{
			Path::Attributes a;

			struct stat sb;
			if (stat(file.c_str(), &sb) < 0)
			{
				a.type = Path::T_NONEXISTANT;
				a.size = 0;
			}
			else
			{
				if (S_ISDIR(sb.st_mode))
					a.type = Path::T_DIRECTORY;
				else
					a.type = Path::T_FILE;

				a.size  = sb.st_size;
			}

			return a;
		}
		bool GetFileList(std::vector<std::string> &list, const std::string &folder, bool recursive)
		{
			DIR *dir;
			struct dirent *ent;

			if ((dir = opendir(folder.c_str())) == nullptr)
			{
				return false;
			}

			while ((ent = readdir(dir)) != nullptr)
			{
				if ((std::strcmp(ent->d_name, ".") == 0) || (std::strcmp(ent->d_name, "..") == 0))
				{
					continue;
				}

				if (ent->d_type == DT_DIR)
				{
					if (recursive)
					{
						GetFileList(list, folder + "/" + ent->d_name, recursive);
					}
				}
				else
				{
					list.push_back(folder + "/" + ent->d_name);
				}
			}

			closedir(dir);

			return true;
		}
	}
}
