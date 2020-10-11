#include "Context.h"
#include <stdio.h>
#include <stdarg.h>
#include "DebugTools.h"
//
namespace debug
{
	Context::Context( const char *file, int line, const char *msg, ... )
		:File( file ),
		Line( line )
	{
		va_list args;
		va_start(args, msg);
		vsnprintf_s(Msg, MAX_CONTEXT_MSG_SIZE, _TRUNCATE, msg, args);
		va_end(args);
		//
		DebugTools::getInstance()->getContextMgr()->pushContext( this );
	}
	Context::~Context()
	{
		DebugTools::getInstance()->getContextMgr()->popContext();
	}
	////////////////////////////////
	ContextManager::ContextManager()
	{
		//
	}
	ContextManager::~ContextManager()
	{
		//
	}
	void ContextManager::getContext()
	{
		//const int BufferSize = 1024;
		//char FinalMessage[BufferSize];
		Context *c;
		std::cout<<"Total contexts: "<<m_contexts.size()<<"\n";
		//
		for( ContextVec::reverse_iterator it1 = m_contexts.rbegin(), it2 = m_contexts.rend(); it1 < it2; ++it1 )
		{
			c = *it1;
			//sprintf_s( FinalMessage, BufferSize, "%s(%d) : %s", c->File, c->Line, c->Msg );
			//std::cout<<FinalMessage<<"\n";
			std::cout<<c->File<<"("<<c->Line<<"): "<<c->Msg<<"\n";
			//MCR_DebugPrint("%s\n", c->Msg);
		}
	}
	bool ContextManager::pushContext( Context *c )
	{
		m_contexts.push_back( c );
		//std::cout<<"Pushed context\n";
		return true;
	}
	Context* ContextManager::popContext()
	{
		Context *c = m_contexts.back();
		m_contexts.pop_back();
		//std::cout<<"Popped context\n";
		return c;
	}
}