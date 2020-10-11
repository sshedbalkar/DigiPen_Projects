#include "Precompiled.h"
#include "Tests.h"
#include <iostream>
#include "FunctionBind.h"
#include "PropertyBind.h"
//
void SimpleFunction10()
{
	std::cout<<"Simple Function10\n";
}
void SimpleFunction11( int a )
{
	std::cout<<"Simple Function11: "<<a<<"\n";
}
void SimpleFunction12( int a, int b )
{
	std::cout<<"Simple Function12: "<<a<<", "<<b<<"\n";
}
void SimpleFunction13( int a, int b, int c )
{
	std::cout<<"Simple Function13: "<<a<<", "<<b<<", "<<c<<"\n";
}
void SimpleFunction14( int a, int b, int c, int d )
{
	std::cout<<"Simple Function14: "<<a<<", "<<b<<", "<<c<<", "<<d<<"\n";
}
/////////////////////////////////////////////////////////////////////
int SimpleFunction20()
{
	std::cout<<"Simple Function20\n";
	return 123;
}
int SimpleFunction21( int a )
{
	std::cout<<"Simple Function21: "<<a<<"\n";
	return a;
}
int SimpleFunction22( int a, int b )
{
	std::cout<<"Simple Function22: "<<a<<", "<<b<<"\n";
	return b;
}
int SimpleFunction23( int a, int b, int c )
{
	std::cout<<"Simple Function23: "<<a<<", "<<b<<", "<<c<<"\n";
	return c;
}
int SimpleFunction24( int a, int b, int c, int d )
{
	std::cout<<"Simple Function24: "<<a<<", "<<b<<", "<<c<<", "<<d<<"\n";
	return d;
}
////////////////////////////////////////////////////////////////////////////
class SimpleClass
{
public:
	SimpleClass(){};
	~SimpleClass(){};
	void func10();
	void func11( float a );
	void func12( float a, float b );
	void func13( float a, float b, float c );
	void func14( float a, float b, float c, float d );
	float func20();
	float func21( float a );
	float func22( float a, float b );
	float func23( float a, float b, float c );
	float func24( float a, float b, float c, float d );
};
//////////////////////////////////////////////////////////////////////////////////////
void SimpleClass::func10()
{
	std::cout<<"SimpleClass::func10\n";
}
void SimpleClass::func11( float a )
{
	std::cout<<"SimpleClass::func11: "<<a<<"\n";
}
void SimpleClass::func12( float a, float b )
{
	std::cout<<"SimpleClass::func12: "<<a<<", "<<b<<"\n";
}
void SimpleClass::func13( float a, float b, float c )
{
	std::cout<<"SimpleClass::func13: "<<a<<", "<<b<<", "<<c<<"\n";
}
void SimpleClass::func14( float a, float b, float c, float d )
{
	std::cout<<"SimpleClass::func14: "<<a<<", "<<b<<", "<<c<<", "<<d<<"\n";
}
///////////////////////////////////////////////////////////////////////////////////
float SimpleClass::func20()
{
	std::cout<<"SimpleClass::func20\n";
	return 123.456f;
}
float SimpleClass::func21( float a )
{
	std::cout<<"SimpleClass::func21: "<<a<<"\n";
	return a;
}
float SimpleClass::func22( float a, float b )
{
	std::cout<<"SimpleClass::func22: "<<a<<", "<<b<<"\n";
	return b;
}
float SimpleClass::func23( float a, float b, float c )
{
	std::cout<<"SimpleClass::func23: "<<a<<", "<<b<<", "<<c<<"\n";
	return c;
}
float SimpleClass::func24( float a, float b, float c, float d )
{
	std::cout<<"SimpleClass::func24: "<<a<<", "<<b<<", "<<c<<", "<<d<<"\n";
	return d;
}
//////////////////////////////////////////////////////////////////////////////
void Test_SimpleFunctionBinding_SimpleFunctionVoid()
{
	ArgumentVector v10;
	FunctionObject *f10 = Bind( &SimpleFunction10 );
	f10->Invoke();
	//
	ArgumentVector v11;
	v11.push_back( 1 );
	FunctionObject *f11 = Bind( &SimpleFunction11 );
	f11->Invoke( NULL, v11 );
	//
	ArgumentVector v12;
	v12.push_back( 1 );
	v12.push_back( 2 );
	FunctionObject *f12 = Bind( &SimpleFunction12 );
	f12->Invoke( NULL, v12 );
	//
	ArgumentVector v13;
	v13.push_back( 1 );
	v13.push_back( 2 );
	v13.push_back( 3 );
	FunctionObject *f13 = Bind( &SimpleFunction13 );
	f13->Invoke( NULL, v13 );
	//
	ArgumentVector v14;
	v14.push_back( 1 );
	v14.push_back( 2 );
	v14.push_back( 3 );
	v14.push_back( 4 );
	FunctionObject *f14 = Bind( &SimpleFunction14 );
	f14->Invoke( NULL, v14 );
}
void Test_SimpleFunctionBinding_SimpleFunctionReturn()
{
	ArgumentVector v20;
	FunctionObject *f20 = Bind( &SimpleFunction20 );
	Variant r20 = f20->Invoke( NULL, v20 );
	std::cout<<"V20: Ret: "<<r20.As<int>()<<"\n";
	//
	ArgumentVector v21;
	v21.push_back( 1 );
	FunctionObject *f21 = Bind( &SimpleFunction21 );
	Variant r21 = f21->Invoke( NULL, v21 );
	std::cout<<"V21: Ret: "<<r21.As<int>()<<"\n";
	//
	ArgumentVector v22;
	v22.push_back( 1 );
	v22.push_back( 2 );
	FunctionObject *f22 = Bind( &SimpleFunction22 );
	Variant r22 = f22->Invoke( NULL, v22 );
	std::cout<<"V22: Ret: "<<r22.As<int>()<<"\n";
	//
	ArgumentVector v23;
	v23.push_back( 1 );
	v23.push_back( 2 );
	v23.push_back( 3 );
	FunctionObject *f23 = Bind( &SimpleFunction23 );
	Variant r23 = f23->Invoke( NULL, v23 );
	std::cout<<"V23: Ret: "<<r23.As<int>()<<"\n";
	//
	ArgumentVector v24;
	v24.push_back( 1 );
	v24.push_back( 2 );
	v24.push_back( 3 );
	v24.push_back( 4 );
	FunctionObject *f24 = Bind( &SimpleFunction24 );
	Variant r24 = f24->Invoke( NULL, v24 );
	std::cout<<"V24: Ret: "<<r24.As<int>()<<"\n";
}
void Test_ClassMethodBinding_ClassMethodVoid()
{
	SimpleClass c1;
	FunctionObject *f30 = Bind( &SimpleClass::func10 );
	f30->Invoke( &c1 );
	//
	ArgumentVector v31;
	v31.push_back( 11.11f );
	FunctionObject *f31 = Bind( &SimpleClass::func11 );
	f31->Invoke( &c1, v31 );
	//
	ArgumentVector v32;
	v32.push_back( 11.11f );
	v32.push_back( 22.22f );
	FunctionObject *f32 = Bind( &SimpleClass::func12 );
	f32->Invoke( &c1, v32 );
	//
	ArgumentVector v33;
	v33.push_back( 11.11f );
	v33.push_back( 22.22f );
	v33.push_back( 33.33f );
	FunctionObject *f33 = Bind( &SimpleClass::func13 );
	f33->Invoke( &c1, v33 );
	//
	ArgumentVector v34;
	v34.push_back( 11.11f );
	v34.push_back( 22.22f );
	v34.push_back( 33.33f );
	v34.push_back( 44.44f );
	FunctionObject *f34 = Bind( &SimpleClass::func14 );
	f34->Invoke( &c1, v34 );
}
void Test_ClassMethodBinding_ClassMethodReturn()
{
	SimpleClass c1;
	FunctionObject *f40 = Bind( &SimpleClass::func20 );
	Variant r40 = f40->Invoke( &c1);
	std::cout<<"V40: Ret: "<<r40.As<float>()<<"\n";
	//
	ArgumentVector v41;
	v41.push_back( 11.11f );
	FunctionObject *f41 = Bind( &SimpleClass::func21 );
	Variant r41 = f41->Invoke( &c1, v41 );
	std::cout<<"V41: Ret: "<<r41.As<float>()<<"\n";
	//
	ArgumentVector v42;
	v42.push_back( 11.11f );
	v42.push_back( 22.22f );
	FunctionObject *f42 = Bind( &SimpleClass::func22 );
	Variant r42 = f42->Invoke( &c1, v42 );
	std::cout<<"V42: Ret: "<<r42.As<float>()<<"\n";
	//
	ArgumentVector v43;
	v43.push_back( 11.11f );
	v43.push_back( 22.22f );
	v43.push_back( 33.33f );
	FunctionObject *f43 = Bind( &SimpleClass::func23 );
	Variant r43 = f43->Invoke( &c1, v43 );
	std::cout<<"V43: Ret: "<<r43.As<float>()<<"\n";
	//
	ArgumentVector v44;
	v44.push_back( 11.11f );
	v44.push_back( 22.22f );
	v44.push_back( 33.33f );
	v44.push_back( 44.44f );
	FunctionObject *f44 = Bind( &SimpleClass::func24 );
	Variant r44 = f44->Invoke( &c1, v44 );
	std::cout<<"V44: Ret: "<<r44.As<float>()<<"\n";
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
class Class1
{
public:
	Class1();
	~Class1();
	int prop1;
};
Class1::Class1()
	:prop1( 123 )
{
	//
}
Class1::~Class1()
{
	//
}
/////////////////////////////////////////////////////
class Class2
{
public:
	Class2();
	~Class2();
	int prop1;
	Class1 *p1;
};
Class2::Class2()
	:p1( new Class1() ),
	prop1( 456 )
{

}
Class2::~Class2()
{
	//
}
/////////////////////////////////////////////////////
void Test_PropertyBind()
{
	Class1 c1;
	PropertyObject *p1 = BindProperty( &c1.prop1 );
	p1->set( 234 );
	int a = p1->get<int>();
	std::cout<<"Prop: "<<c1.prop1<<", Bound: "<<p1->get<int>()<<"\n";
	//
	Class2 c2;
	PropertyObject *p2 = BindProperty( &c2.p1 );
	Class1 *t1 = p2->get<Class1*>();
	std::cout<<"Val: "<<t1->prop1<<", "<<c2.p1->prop1<<"\n";
	t1->prop1 = 23453;
	std::cout<<"Val: "<<t1->prop1<<", "<<c2.p1->prop1<<"\n";
	p2->set( &c1 );
	std::cout<<"Val: "<<t1->prop1<<", "<<c2.p1->prop1<<"\n";
}
namespace Framework
{
	TestClass::TestClass()
	{
		//
	}
	//
	TestClass::~TestClass()
	{
		//
	}
	//
	void TestClass::Run()
	{
		std::cout<<"Testing Function Binding:\n";
		Test_SimpleFunctionBinding_SimpleFunctionVoid();
		std::cout<<"///////////////////////////////////////\n";
		Test_SimpleFunctionBinding_SimpleFunctionReturn();
		std::cout<<"///////////////////////////////////////\n";
		Test_ClassMethodBinding_ClassMethodVoid();
		std::cout<<"///////////////////////////////////////\n";
		Test_ClassMethodBinding_ClassMethodReturn();
		std::cout<<"///////////////////////////////////////\n";
		std::cout<<"\n\n\nTesting Property Binding:\n";
		Test_PropertyBind();
	}
}