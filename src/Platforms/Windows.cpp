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

#include <Windows.h>

namespace PZ
{
	namespace Platform
	{
		double GetHighResolutionClock()
		{
			LARGE_INTEGER freq, t;

			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&t);

			return (static_cast<double>(t.QuadPart) / static_cast<double>(freq.QuadPart));
		}

		Path::Attributes GetAttributes(const std::string &file)
		{
			Path::Attributes a;

			HANDLE hFile = CreateFileA(file.c_str(), 0, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				a.type = Path::T_NONEXISTANT;
				a.size = 0;
			}
			else
			{
				BY_HANDLE_FILE_INFORMATION info;
				GetFileInformationByHandle(hFile, &info);

				if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					a.type = Path::T_DIRECTORY;
				else
					a.type = Path::T_FILE;

				a.size = info.nFileSizeLow;
			}

			CloseHandle(hFile);

			return a;
		}
		bool GetFileList(std::vector<std::string> &list, const std::string &folder, bool recursive)
		{
			std::string searchQuery = folder + "/*";
			WIN32_FIND_DATAA FindFileData;
			HANDLE hFind;

			hFind = FindFirstFileA(searchQuery.c_str(), &FindFileData);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			do
			{
				if ((std::strcmp(FindFileData.cFileName, ".") == 0) || (std::strcmp(FindFileData.cFileName, "..") == 0))
				{
					continue;
				}

				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (recursive)
					{
						GetFileList(list, folder + "/" + FindFileData.cFileName, recursive);
					}
				}
				else
				{
					list.push_back(folder + "/" + FindFileData.cFileName);
				}
			} while (FindNextFileA(hFind, &FindFileData));

			FindClose(hFind);

			return true;
		}
	}
}
