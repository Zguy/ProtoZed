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
#include <ProtoZed/Assets/AssetGroupAsset.h>

#include <ProtoZed/Application.h>
#include <ProtoZed/AssetManager.h>

namespace PZ
{
	AssetGroupAsset::AssetGroupAsset()
	{
	}
	AssetGroupAsset::~AssetGroupAsset()
	{
	}


	void AssetGroupAsset::loadGroup(const Jzon::Object &groupObj)
	{
		AssetGroup group;

		const Jzon::Node &nameNode = groupObj.Get("Name", Jzon::Value(""));
		if (nameNode.IsString())
			group.name = nameNode.AsString();
		else
			return;

		const Jzon::Node &assetsNode = groupObj.Get("Assets", Jzon::Array());
		if (!assetsNode.IsArray())
			return;

		const Jzon::Array &assets = assetsNode.AsArray();
		for (Jzon::Array::const_iterator it = assets.begin(); it != assets.end(); ++it)
		{
			if (!(*it).IsString())
				return;

			std::string file = (*it).AsString();
			group.files.push_back(file);
		}

		if (GetApplication().GetAssetManager().AddGroup(group))
		{
			loadedGroups.push_back(group.name);
		}
	}
	bool AssetGroupAsset::load(const DataChunk &data)
	{
		std::string str(data.GetData(), data.GetSize());

		Jzon::Node::Type type = Jzon::Node::DetermineType(str);
		if (type == Jzon::Node::T_OBJECT)
		{
			Jzon::Object root;
			root.Read(str);
			loadGroup(root);

			return true;
		}
		else if (type == Jzon::Node::T_ARRAY)
		{
			Jzon::Array root;
			root.Read(str);

			for (Jzon::Array::iterator it = root.begin(); it != root.end(); ++it)
			{
				if ((*it).IsObject())
				{
					loadGroup((*it).AsObject());
				}
			}

			return true;
		}

		return false;
	}

	bool AssetGroupAsset::unload()
	{
		for (std::vector<std::string>::const_iterator it = loadedGroups.cbegin(); it != loadedGroups.cend(); ++it)
		{
			GetApplication().GetAssetManager().RemoveGroup(*it);
		}
		loadedGroups.clear();
		return true;
	}
}