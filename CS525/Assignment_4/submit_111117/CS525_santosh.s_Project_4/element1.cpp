#include "element1.h"

CS225::Element1::Element1(int _val) : AbstractElement(), val(_val) { }

CS225::Element1::Element1(const Element1& orig):
	AbstractElement(),
	val(orig.val)
{
	//
}

CS225::Element1::~Element1()
{
	//
}
CS225::Element1* CS225::Element1::clone() const
{
	return new Element1(*this);
}
int CS225::Element1::Get() const { return val; }

void CS225::Element1::Set(int new_val) { val=new_val; }

void CS225::Element1::Print() const { 
  std::cout << "[" << Get() << "] "; 
}
