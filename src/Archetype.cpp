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
#include <ProtoZed/Archetype.h>

#include <ProtoZed/Component.h>
#include <ProtoZed/Log.h>

namespace PZ
{
	Archetype::Archetype()
	{
	}
	Archetype::~Archetype()
	{
	}

	bool Archetype::Create(EntityManager &manager, const EntityID &id) const
	{
		if (manager.CreateEntity(id))
		{
			for (ComponentList::const_iterator component_it = components.cbegin(); component_it != components.cend(); ++component_it)
			{
				const ComponentPropertyPair &cpp = (*component_it);

				Component *component = manager.AddComponent(id, cpp.first);
				if (component == nullptr)
				{
					Log::Error("ProtoZed", "Creating archetype \""+name+"\" - Component \""+cpp.first+"\" could not be added");
				}

				for (PropertyValueList::const_iterator property_it = cpp.second.cbegin(); property_it != cpp.second.cend(); ++property_it)
				{
					const PropertyValuePair &pvp = (*property_it);

					component->GetProperty(pvp.first).FromString(pvp.second);
				}
			}

			return true;
		}

		return false;
	}
}
