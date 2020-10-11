//
#include "Object.h"
#include "Utility.h"
//
namespace Wye
{
	Object::Object(int ObjectId): 
		_id(ObjectId)
	{
		//
	}
	Object::Object(): 
		_id(getUniqueId())
	{
		//
	}
	Object::Object(const Object& orig): 
		_id(orig._id)
	{
		//
	}
	Object::~Object()
	{
		//
	}
	Object::operator int() const
	{
		return _id;
	}
	Object& Object::operator =(const Object& rhs)
	{
		_id = rhs._id;
		return *this;
	}
	//
	int Object::getId() const
	{
		return _id;
	}
}