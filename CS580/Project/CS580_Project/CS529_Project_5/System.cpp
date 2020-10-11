//#pragma message("Compiling: "__FILE__)
//
#include "System.h"
#include "ComponentTypeIds.h"
//
namespace Wye
{
	System::System():
		Composition()
	{
		setType(CT_System);
		_name = "System";
	}
	System::~System()
	{
		//
	}
	void System::update(float timeslice)
	{
		//
	}
}