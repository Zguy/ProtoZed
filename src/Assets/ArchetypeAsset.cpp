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
#include <ProtoZed/Assets/ArchetypeAsset.h>

#include <ProtoZed/Application.h>
#include <ProtoZed/Archetype.h>
#include <ProtoZed/Jzon.h>

namespace PZ
{
	ArchetypeAsset::ArchetypeAsset()
	{
	}
	ArchetypeAsset::~ArchetypeAsset()
	{
	}

	bool ArchetypeAsset::loadData(const DataChunk &data, Archetype *archetype)
	{
		std::string str(data.GetData(), data.GetSize());

		if (Jzon::Node::DetermineType(str) != Jzon::Node::T_OBJECT)
			return false;

		Jzon::Object root;
		Jzon::Parser parser(root, str);
		parser.Parse();

		const Jzon::Node &nameNode = root.Get("Name");
		if (nameNode.IsString())
			archetype->name = nameNode.ToString();
		else
			return false;

		const Jzon::Node &componentsNode = root.Get("Components");
		if (!componentsNode.IsArray())
			return false;

		const Jzon::Array &componentsRoot = componentsNode.AsArray();
		for (Jzon::Array::const_iterator it = componentsRoot.begin(); it != componentsRoot.end(); ++it)
		{
			std::string name;
			const Jzon::Node &nameNode = (*it).Get("Name");
			if (nameNode.IsString())
				name = nameNode.ToString();
			else
				return false;

			Archetype::PropertyValueList properties;

			const Jzon::Node &propertiesNode = (*it).Get("Properties");
			if (!propertiesNode.IsObject())
				return false;

			const Jzon::Object &propertiesRoot = propertiesNode.AsObject();
			for (Jzon::Object::const_iterator it = propertiesRoot.begin(); it != propertiesRoot.end(); ++it)
			{
				std::string name = (*it).first;
				const Jzon::Node &valueNode = (*it).second;
				std::string value = valueNode.ToString();

				properties.push_back(std::make_pair(name, value));
			}

			archetype->components.push_back(std::make_pair(name, properties));
		}

		return true;
	}

	bool ArchetypeAsset::load(const DataChunk &data)
	{
		Archetype *archetype = new Archetype;

		if (loadData(data, archetype))
		{
			name = archetype->name;
			return GetApplication().GetEntityManager().RegisterArchetype(archetype);
		}

		return false;
	}
	bool ArchetypeAsset::unload()
	{
		bool success = GetApplication().GetEntityManager().UnregisterArchetype(name);
		name.clear();
		return success;
	}
}
