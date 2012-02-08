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
#include <ProtoZed/Components/SceneNode.h>

#include <ProtoZed/Log.h>
#include <ProtoZed/Messages.h>

namespace PZ
{
	const HashString SceneNode::Family = "SceneNode";

	void SceneNode::SetParent(const EntityID &id)
	{
		if (id == EntityID())
		{
			parent = EntityID();
		}
		else
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
		if (id != EntityID())
		{
			if (!GetManager().HasComponent<SceneNode>(id))
			{
				Log::Error("ProtoZed", "The child of an entity must also have the SceneNode component");
			}
			else if (!HasChild(id))
			{
				children.push_back(id);

				SceneNode *node = GetManager().GetComponent<SceneNode>(id);
				node->_SetParent(GetOwnerID());

				GetManager().SendMessage(ChildMessage::Create(id, true), GetOwnerID());
			}
		}
	}
	void SceneNode::RemoveChild(const EntityID &id)
	{
		if (id != EntityID())
		{
			for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
			{
				if ((*it) == id)
				{
					children.erase(it);

					SceneNode *node = GetManager().GetComponent<SceneNode>(id);
					node->SetParent(EntityID());

					GetManager().SendMessage(ChildMessage::Create(id, false), GetOwnerID());

					break;
				}
			}
		}
	}
	bool SceneNode::HasChild(const EntityID &id)
	{
		for (EntityList::const_iterator it = children.begin(); it != children.end(); ++it)
		{
			if ((*it) == id)
			{
				return true;
			}
		}

		return false;
	}

	void SceneNode::_SetParent(const EntityID &id)
	{
		if (id == EntityID())
		{
			parent = EntityID();
		}
		else
		{
			if (parent != EntityID()) // If we already have a parent
			{
				SceneNode *node = GetManager().GetComponent<SceneNode>(parent);
				node->_RemoveChild(GetOwnerID());
			}

			parent = id;
		}
	}
	void SceneNode::_AddChild(const EntityID &id)
	{
		if (id != EntityID())
		{
			if (!HasChild(id))
			{
				children.push_back(id);

				GetManager().SendMessage(ChildMessage::Create(id, true), GetOwnerID());
			}
		}
	}
	void SceneNode::_RemoveChild(const EntityID &id)
	{
		if (id != EntityID())
		{
			for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
			{
				if ((*it) == id)
				{
					children.erase(it);

					GetManager().SendMessage(ChildMessage::Create(id, false), GetOwnerID());

					break;
				}
			}
		}
	}
}