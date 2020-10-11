#pragma once
//
namespace debug
{
	enum LogLevel
	{
		LogError,
		LogWarning,
		LogInfo,
		LogDebug0,
		LogDebug1,
		LogDebug2,
		LogDebug3,
		LogDebug4
	};
	//
	class MsgBuffer;
	//
	class Logger
	{
	public:
		Logger();
		~Logger();
		//
		void Log();
		int LogThreshold();
		void LogThreshold( const int& lvl );
		//
	private:
		int m_logThreshold;
		MsgBuffer *m_buffer;
	};
}