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
#ifndef PZ_Path_h__
#define PZ_Path_h__

#include <string>

namespace PZ
{
	/**
	 * \brief A file path.
	 * 
	 * All paths are guaranteed to:
	 * - Not contain "." or ".." (except when pointing to the current directory).
	 * - Use "/" as separator.
	 * - Not have a trailing "/".
	 */
	class Path
	{
	public:
		enum Type
		{
			T_NONEXISTANT,
			T_FILE,
			T_DIRECTORY
		};
		struct Attributes
		{
			Type type;
			std::size_t size;
		};

		Path();
		Path(const Path &other);
		Path(const std::string &path);
		Path(const char *path);
		~Path();

		Path &operator=(const Path &other);
		Path &operator=(const std::string &path);
		Path &operator=(const char *path);

		inline bool operator==(const Path &other) const
		{
			return (path == other.path);
		}
		inline bool operator!=(const Path &other) const
		{
			return (path != other.path);
		}

		inline bool operator<(const Path &other) const
		{
			return (path < other.path);
		}
		inline bool operator>(const Path &other) const
		{
			return (path > other.path);
		}

		Path &operator+=(const Path &other);
		const Path operator+(const Path &other) const
		{
			return Path(*this) += other;
		}

		Attributes GetAttributes() const;

		bool Exists() const;

		inline bool IsEmpty() const
		{
			return path.empty();
		}
		inline void Clear()
		{
			path.clear();
		}

		const std::string &ToString() const
		{
			return path;
		}

	private:
		void normalize();

		std::string path;
	};
}

#endif // PZ_Path_h__
