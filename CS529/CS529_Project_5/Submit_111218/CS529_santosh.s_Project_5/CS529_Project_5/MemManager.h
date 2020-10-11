#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H
#pragma message("Compiling: "__FILE__)
//
//#include "Engine.h"
//
//namespace Wye
//{
	inline void* operator new(size_t size, const char* file, int line);
	inline void* operator new[](size_t size, const char* file, int line);
	inline void operator delete(void* pointer);
	inline void operator delete[](void* pointer);
//}

#ifdef _DEBUG
//#define DEBUG_NEW new(__FILE__, __LINE__)
#define DEBUG_NEW new
#else
#define DEBUG_NEW new
#endif
#define new DEBUG_NEW
	//
#endif