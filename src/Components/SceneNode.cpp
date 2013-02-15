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
#include <ProtoZed/Components/SceneNode.h>

#include <ProtoZed/Log.h>

namespace PZ
{
	const HashString SceneNode::Family = "SceneNode";

	SceneNode::SceneNode()
	{
	}
	SceneNode::~SceneNode()
	{
	}

	void SceneNode::Destroy()
	{
		if (parent != EntityID())
		{
			SceneNode *node = GetManager().GetComponent<SceneNode>(parent);
			if (node != nullptr)
				node->_RemoveChild(GetOwnerID());

			parent = EntityID();
		}
		for (EntitySet::iterator it = children.begin(); it != children.end(); ++it)
		{
			SceneNode *node = GetManager().GetComponent<SceneNode>(*it);
			if (node != nullptr)
				node->_SetParent(EntityID(), false);
		}
		children.clear();
	}

	void SceneNode::SetParent(const EntityID &id)
	{
		if (id != GetOwnerID())
		{
			if (!GetManager().HasComponent<SceneNode>(id))
			{
				Log::Error("ProtoZed", "The parent of an entity must also have the SceneNode component");
			}
			else
			{
				if (parent != EntityID()) // If we already have a parent
				{
					SceneNode *node = GetManager().GetComponent<SceneNode>(parent);
					node->_RemoveChild(GetOwnerID());
				}

				parent = id;
				UpdateTimestamp();

				if (parent != EntityID())
				{
					SceneNode *node = GetManager().GetComponent<SceneNode>(parent);
					node->_AddChild(GetOwnerID());
				}
			}
		}
	}

	void SceneNode::AddChild(const EntityID &id)
	{
		if (id != EntityID() && id != GetOwnerID())
		{
			if (!GetManager().HasComponent<SceneNode>(id))
			{
				Log::Error("ProtoZed", "The child of an entity must also have the SceneNode component");
			}
			else if (!HasChild(id))
			{
				children.insert(id);
				UpdateTimestamp();

				SceneNode *node = GetManager().GetComponent<SceneNode>(id);
				node->_SetParent(GetOwnerID());
			}
		}
	}
	void SceneNode::RemoveChild(const EntityID &id)
	{
		if (id != EntityID() && id != GetOwnerID())
		{
			EntitySet::iterator it = children.find(id);
			if (it != children.end())
			{
				children.erase(it);
				UpdateTimestamp();

				SceneNode *node = GetManager().GetComponent<SceneNode>(id);
				node->_SetParent(EntityID(), false);
			}
		}
	}
	bool SceneNode::HasChild(const EntityID &id) const
	{
		return (children.find(id) != children.cend());
	}

	void SceneNode::_SetParent(const EntityID &id, bool removeFromOldParent)
	{
		if (removeFromOldParent && parent != EntityID()) // If we already have a parent
		{
			SceneNode *node = GetManager().GetComponent<SceneNode>(parent);
			node->_RemoveChild(GetOwnerID());
		}

		parent = id;
		UpdateTimestamp();
	}
	void SceneNode::_AddChild(const EntityID &id)
	{
		if (id != EntityID())
		{
			if (!HasChild(id))
			{
				children.insert(id);
				UpdateTimestamp();
			}
		}
	}
	void SceneNode::_RemoveChild(const EntityID &id)
	{
		if (id != EntityID())
		{
			EntitySet::iterator it = children.find(id);
			if (it != children.end())
			{
				children.erase(it);
				UpdateTimestamp();
			}
		}
	}
}