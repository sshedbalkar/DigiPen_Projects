#include "Message.h"
//
Message::Message( void )
	:ID( MSG_INVALID ),
	Obj( 0 )
{
	//
}
Message::Message( const Message &orig )
	:ID( orig.ID ),
	Obj( orig.Obj )
{
	//
}
Message::Message( const MSG_Id &id, Object obj )
	:ID( id ),
	Obj( obj )
{
	//
}
Message::~Message()
{
	//
}
Message& Message::operator= ( const Message &rhs )
{
	if( this != &rhs )
	{
		ID = rhs.ID;
		Obj = rhs.Obj;
	}
	return *this;
}
const char*	Message::GetName()
{
	return MessageNameText[ ID ];
}