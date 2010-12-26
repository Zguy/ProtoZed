/*
	Copyright 2010 Johannes Häggqvist

	This file is part of ProtoZed.

	ProtoZed is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ProtoZed is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with ProtoZed.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ProtoZed/UniqueIDGenerator.h"

namespace PZ
{
	const UniqueID UniqueIDGenerator::GetNextID(const std::string &category)
	{
		if (currentID.find(category) == currentID.end())
		{
			currentID[category] = 0;
		}

		return currentID[category]++;
	}

	std::map<std::string, UniqueID> UniqueIDGenerator::currentID;
}