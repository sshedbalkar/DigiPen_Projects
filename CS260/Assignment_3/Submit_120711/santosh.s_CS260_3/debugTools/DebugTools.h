////////////////////////////////////////////////////////////////
//
//	DebugTools.h
//	Collection of all the available debug tools. Although there aren't many tools in this class at the moment(only debug stream). Planning on expanding in the future.
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#pragma once
//#pragma message("Compiling: "__FILE__)
//
#include "Assert.h"
//
#define DEBUG_OUTPUT_FILE "debugOutput.txt"
//
namespace debug
{
	class DebugStream;
	class ContextManager;
	//
	class DebugTools
	{
	private:
		void init();
		//
		static DebugTools* _instance;
		DebugStream *m_dStream;
		ContextManager *m_contextMgr;
		//
	public:
		DebugTools();
		~DebugTools();
		static DebugTools* getInstance()
		{
			MCR_ErrorIf( _instance == 0, "DebugTools instance is not created!" );
			return _instance;
		}
		void enable(bool b);
		inline DebugStream* getDebugStream()
		{
			return m_dStream;
		}
		inline ContextManager* getContextMgr()
		{
			return m_contextMgr;
		}
	};
}
