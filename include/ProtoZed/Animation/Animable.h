#ifndef Animable_h__
#define Animable_h__

#include <memory>

namespace PZ
{
	class Animable
	{
	public:
		typedef unsigned int Attribute;

		Animable()
		{}
		~Animable()
		{}

		virtual bool SupportAttribute(Attribute attribute) = 0;

		virtual void SetAttribute(Attribute attribute, float value) = 0;
		virtual float GetAttribute(Attribute attribute) = 0;
	};

	typedef std::shared_ptr<Animable> AnimablePtr;
}

#endif // Animable_h__