#pragma once
//
namespace debug
{
	class MsgBuffer
	{
	public:
		virtual void PushBack( const char *msg ) = 0;
		virtual const char* PopFront() = 0;
		virtual bool IsEmpty() = 0;
	};
}