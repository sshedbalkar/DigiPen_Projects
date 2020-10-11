#pragma once
//
#include "../common/Variant.h"
//
/////////////////////////////////////////////////////
//Macro trick to make message names enums
//from the file MsgNames.h
#define MCR_REGISTER_MESSAGE_NAME(x) x,
typedef enum
{
	MSG_INVALID = 0,
	#include "MsgNames.h"
	MSG_NUM
} MSG_Id;
#undef MCR_REGISTER_MESSAGE_NAME
///////////////////////////////////////////////////
#define MCR_REGISTER_MESSAGE_NAME(x) #x,
static const char* MessageNameText[] =
{
	"MSG_Invalid",
	#include "MsgNames.h"
	"Invalid_Max_Num"
};
#undef MCR_REGISTER_MESSAGE_NAME
///////////////////////////////////////////////////
//
class Message
{
public:
				Message( void );
				Message( const Message &orig );
				Message( const MSG_Id &id, Object obj = 0 );
	virtual		~Message();
	Message&	operator= ( const Message &rhs );
	//
	const char*	GetName();
	//
	MSG_Id		ID;
	Object		Obj;
};