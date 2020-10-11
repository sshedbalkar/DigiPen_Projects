#pragma message("Compiling: "__FILE__)
//
#include <iostream>
//#include "Engine.h"
//
//namespace Wye
//{
	void* operator new(size_t size, const char* file, int line)
	{
		std::cout<<"operator new1: "<<size<<" Bytes, File: "<<file<<", Line: "<<line<<"\n";
		void* m = malloc(size);
		if(!m) std::cout<<"out of memory";
		return m;
	}

	void* operator new[](unsigned int size, const char* file, int line)
	{
		std::cout<<"operator new2: "<<size<<" Bytes, File: "<<file<<", Line: "<<line<<"\n";
		void* m = malloc(size);
		if(!m) std::cout<<"out of memory";
		return m;
	}

	void operator delete(void* pointer)
	{
		std::cout<<"Deleting: "<<pointer<<"\n";
		free(pointer);
	}

	void operator delete[](void* pointer)
	{
		//std::cout<<"operator delete 2\n";
		free(pointer);
	}
//}