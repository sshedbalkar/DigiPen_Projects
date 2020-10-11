#include "DebugTools.h"
#include "DebugPrint.h"
#include "Context.h"
//
#include "DebugInit.h"
//
namespace debug
{
	DebugTools* DebugTools::_instance = 0;
	//
	DebugTools::DebugTools()
		:m_dStream( NULL ),
		m_contextMgr( NULL )
	{
		MCR_ErrorIf( _instance != 0, "DebugTools instance is already created!" );
		init();
		//
		_instance = this;
	}
	DebugTools::~DebugTools()
	{
		delete m_dStream;
		delete m_contextMgr;
		//
		m_dStream = NULL;
		m_contextMgr = NULL;
		//
		_instance = 0;
	}
	//
	void DebugTools::init()
	{
		debug::init();
		m_dStream = new DebugStream( DEBUG_OUTPUT_FILE );
		m_contextMgr = new ContextManager();
	}
	void DebugTools::enable(bool b)
	{
		m_dStream->enable(b);
	}
}