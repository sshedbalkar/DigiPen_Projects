#ifndef NON_TYPE_DELEGATE_H
#define NON_TYPE_DELEGATE_H
#pragma message("Compiling: "__FILE__)
//
#include <iostream>
#include "Delegate.h"
namespace Wye
{
	template <typename T>
	class NonTypeDelegate : public Delegate 
	{
	public:
		NonTypeDelegate(void (*pfn)(T)):
			Delegate(),
			_t(t),
			_pfn(pfn)
		{
			//
		};
		~NonTypeDelegate()
		{
			//
		};
		void invoke(T param)
		{
			std::cout << "NonTypeDelegate Invoke\r\n";
			(*_pfn)(param);
		}

	private:
		T _t;
		void (*_pfn)(T);
	};
}
//
#endif