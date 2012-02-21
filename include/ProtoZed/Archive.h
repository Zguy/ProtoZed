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
#ifndef PZ_Archive_h__
#define PZ_Archive_h__

#include <string>

namespace PZ
{
	class DataChunk
	{
	public:
		DataChunk() : data(nullptr), size(0), references(new unsigned int(1))
		{}
		DataChunk(const char *data, unsigned int size) : data(data), size(size), references(new unsigned int(1))
		{}
		DataChunk(const DataChunk &other) : data(other.data), size(other.size), references(other.references)
		{
			addReference();
		}
		~DataChunk()
		{
			release();
		}

		const DataChunk &operator=(const DataChunk &other)
		{
			if (this != &other)
			{
				release();

				data = other.data;
				size = other.size;

				references = other.references;
				addReference();
			}

			return *this;
		}

		const char *GetData() const
		{
			return data;
		}

		unsigned int GetSize() const
		{
			return size;
		}

	private:
		const char *data;
		unsigned int size;

		void addReference()
		{
			++(*references);
		}
		void release()
		{
			if (--(*references) == 0)
			{
				delete[] data;
				data = nullptr;
				delete references;
				references = nullptr;
			}
		}
		unsigned int *references;
	};

	class Archive
	{
	public:
		Archive()
		{}
		virtual ~Archive()
		{}

		virtual bool Open(const std::string &filename) = 0;
		virtual bool Close() = 0;
		virtual bool IsOpen() = 0;

		virtual DataChunk Get(const std::string &filename) const = 0;

		virtual bool Has(const std::string &filename) const = 0;
	};
}

#endif // PZ_Archive_h__
