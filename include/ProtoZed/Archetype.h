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
#ifndef PZ_Archetype_h__
#define PZ_Archetype_h__

#include <ProtoZed/EntityManager.h>

#include <vector>

namespace PZ
{
	class Archetype
	{
	public:
		Archetype();
		~Archetype();

		bool Create(EntityManager &manager, const EntityID &id) const;

		typedef std::pair<std::string, std::string> PropertyValuePair;
		typedef std::vector<PropertyValuePair> PropertyValueList;
		typedef std::pair<std::string, PropertyValueList> ComponentPropertyPair;
		typedef std::vector<ComponentPropertyPair> ComponentList;

		std::string name;
		std::string parent;
		ComponentList components;

	private:
		void imp_create(EntityManager &manager, const EntityID &id) const;
	};
}

#endif // PZ_Archetype_h__
