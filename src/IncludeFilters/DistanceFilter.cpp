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
#include <ProtoZed/IncludeFilters/DistanceFilter.h>

#include <ProtoZed/Components/Position2D.h>

namespace PZ
{
	DistanceFilter::DistanceFilter(const Vector2f &origin, const float distance) : origin(origin), distance(distance)
	{
	}
	DistanceFilter::~DistanceFilter()
	{
	}

	bool DistanceFilter::TestEntity(const MetaEntity &entity) const
	{
		Position2D *position = entity.GetComponent<Position2D>();
		if (position == nullptr)
			return false;
		else
			return (origin - position->GetPosition()).GetLengthSquared() <= (distance*distance);
	}
}