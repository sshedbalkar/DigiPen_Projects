#ifndef DELEGATE_H
#define DELEGATE_H
#pragma message("Compiling: "__FILE__)
//
#include "Engine.h"
namespace Wye
{
	class Delegate:public Object
	{
	public:
		virtual invoke() = 0;
	protected:
		Delegate():
			Object(getUniqueId())
		{
			//
		};
		~Delegate()
		{
			//
		};
	};
}
//
#endif