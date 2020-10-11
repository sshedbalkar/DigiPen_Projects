#include "Logger.h"
#include "MsgBuffer.h"
#include <ctime>
//
namespace debug
{
	Logger::Logger()
	{
		//
	}
	Logger::~Logger()
	{
		//
	}
	void Logger::Log()
	{
		unsigned int c = ( unsigned int ) clock();

	}
	int Logger::LogThreshold()
	{
		return m_logThreshold;
	}
	void Logger::LogThreshold( const int& lvl )
	{
		m_logThreshold = lvl;
	}
}