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

namespace PZ
{
	ArchetypeAsset::ArchetypeAsset()
	{
	}
	ArchetypeAsset::~ArchetypeAsset()
	{
	}

	bool ArchetypeAsset::loadArchetype(const Jzon::Object &archetypeObj)
	{
		Archetype *archetype = new Archetype;

		const Jzon::Node &nameNode = archetypeObj.Get("Name");
		if (nameNode.IsString())
			archetype->name = nameNode.ToString();
		else
			return false;

		const Jzon::Node &componentsNode = archetypeObj.Get("Components");
		if (!componentsNode.IsArray())
			return false;

		const Jzon::Array &componentsRoot = componentsNode.AsArray();
		for (Jzon::Array::const_iterator it = componentsRoot.begin(); it != componentsRoot.end(); ++it)
		{
			const Jzon::Node &nameNode = (*it).Get("Name");
			if (!nameNode.IsString())
				return false;

			std::string name = nameNode.ToString();

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

		if (GetApplication().GetEntityManager().RegisterArchetype(archetype))
		{
			loadedArchetypes.push_back(archetype->name);
		}

		return true;
	}
	bool ArchetypeAsset::load(const DataChunk &data)
	{
		std::string str(data.GetData(), data.GetSize());

		Jzon::Node::Type type = Jzon::Node::DetermineType(str);
		if (type == Jzon::Node::T_OBJECT)
		{
			Jzon::Object root;
			Jzon::Parser parser(root, str);
			parser.Parse();
			return loadArchetype(root);
		}
		else if (type == Jzon::Node::T_ARRAY)
		{
			Jzon::Array root;
			Jzon::Parser parser(root, str);
			parser.Parse();

			for (Jzon::Array::iterator it = root.begin(); it != root.end(); ++it)
			{
				if ((*it).IsObject())
				{
					loadArchetype((*it).AsObject());
				}
			}

			return true;
		}

		return false;
	}

	bool ArchetypeAsset::unload()
	{
		for (std::vector<std::string>::const_iterator it = loadedArchetypes.cbegin(); it != loadedArchetypes.cend(); ++it)
		{
			GetApplication().GetEntityManager().UnregisterArchetype(*it);
		}
		loadedArchetypes.clear();
		return true;
	}
}
