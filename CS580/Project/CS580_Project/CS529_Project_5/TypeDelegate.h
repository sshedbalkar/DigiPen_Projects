#ifndef TYPE_DELEGATE_H
#define TYPE_DELEGATE_H
#pragma message("Compiling: "__FILE__)
//
#include <iostream>
#include "Delegate.h"
namespace Wye
{
	template <typename T1, typename T2>
	class TypeDelegate : public Delegate 
	{
	public:
		TypeDelegate(T1 &t, void (T1::*pfn)(T2)):
			Delegate(),
			_t(t),
			_pfn(pfn)
		{
			//
		};
		~TypeDelegate()
		{
			//
		};
		void invoke(T2 param)
		{
			std::cout << "TypeDelegate Invoke\r\n";
			(_t.*_pfn)(param);
		}

	private:
		T1 _t;
		void (T1::*_pfn)(T2);
	};
}
//
#endif