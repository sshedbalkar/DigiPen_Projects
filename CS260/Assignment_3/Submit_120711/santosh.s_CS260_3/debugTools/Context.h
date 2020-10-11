#pragma once
//
#include <iostream>
#include <vector>
#include "../utility//Utility.h"
//
namespace debug
{
	#define MAX_CONTEXT_MSG_SIZE 512
	//
	class DebugTools;
	//
	class Context
	{
	public:
		Context( const char *file, int line, const char *msg = 0, ... );
		~Context();
		//
		const char *File;
		int Line;
		char Msg[MAX_CONTEXT_MSG_SIZE];
	};
	//
	class ContextManager
	{
	private:
		ContextManager();
		~ContextManager();
		friend class DebugTools;
		//
	public:
		void getContext();
		bool pushContext( Context *c );
		Context* popContext();
		//
	private:
		typedef std::vector< Context* > ContextVec;
		ContextVec m_contexts;
	};
	//
	#if ENABLE_DEBUG_DIAGNOSTICS
		#define MCR_PushContext( ... ) \
			debug::Context MCR_Concatenate( C_, __COUNTER__ )( __FILE__, __LINE__, __VA_ARGS__ )
	#else
		#define MCR_PushContext(...) ((void)0)
	#endif
}