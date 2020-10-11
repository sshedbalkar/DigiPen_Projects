#include "Tester.h"
#include <iostream>
#include "Os.hpp"
#include "JobSystem.hpp"
#include "Thread.hpp"
#include "common\FunctionBind.h"
#include "TestJob1.h"
#include "JobFromFunction.h"
#include "ComplexJob.h"
#include "JobGroup.h"
//
void SimpleFunction11( int a )
{
	std::cout<<"Simple Function11: "<<a<<"\n";
}
/////////////////////////////////////////////////////////////////////
Tester::Tester()
{

}
Tester::~Tester()
{

}
void Tester::Test()
{
	GenerateJobFromFunc();
	PriorityTest();
	ComplexJobTest();
	JobGroupTest();
}
void Tester::JobGroupTest()
{
	std::cout<<"//////////////////////////////Started Job Group Test\n";
	Job* j1 = new TestJob1( rand() % 3 );
	Job* j2 = new TestJob1( rand() % 3 );
	Job* j3 = new TestJob1( rand() % 3 );
	//
	ArgumentVector v11;
	v11.push_back( 123 );
	FunctionObject *f11 = Bind( &SimpleFunction11 );
	//
	Job *j4 = new JobFromFunction( f11, v11 );
	//
	JobGroup *cj = new JobGroup();
	cj->Priority = 5;
	cj->AddChildJob( j1 );
	cj->AddChildJob( j2 );
	cj->AddChildJob( j3 );
	cj->AddChildJob( j4 );
	//
	JobSystem::GetSingleton().Add(j1);
	JobSystem::GetSingleton().Add(j2);
	JobSystem::GetSingleton().Add(j3);
	JobSystem::GetSingleton().Add(j4);
	JobSystem::GetSingleton().Add((Job*)cj);
}
void Tester::ComplexJobTest()
{
	std::cout<<"////////////////////////////Started Complex Job Test\n";
	Job* j1 = new TestJob1( rand() % 3 );
	Job* j2 = new TestJob1( rand() % 3 );
	Job* j3 = new TestJob1( rand() % 3 );
	//
	ArgumentVector v11;
	v11.push_back( 123 );
	FunctionObject *f11 = Bind( &SimpleFunction11 );
	//
	Job *j4 = new JobFromFunction( f11, v11 );
	//
	ComplexJob *cj = new ComplexJob();
	cj->Priority = 5;
	cj->AddChildJob( j1 );
	cj->AddChildJob( j2 );
	cj->AddChildJob( j3 );
	cj->AddChildJob( j4 );
	//
	JobSystem::GetSingleton().Add(j1);
	JobSystem::GetSingleton().Add(j2);
	JobSystem::GetSingleton().Add(j3);
	JobSystem::GetSingleton().Add(j4);
	JobSystem::GetSingleton().Add((Job*)cj);
}
void Tester::PriorityTest()
{
	std::cout<<"//////////////////////////////Started Jobs with Priority test\n";
	int max = 20;
	for(unsigned int i=0;i<max;++i)
	{
		Job* j = new TestJob1( rand() % 3 );
		j->Priority = i;
		JobSystem::GetSingleton().Add(j);
	}
}
void Tester::GenerateJobFromFunc()
{
	std::cout<<"//////////////////////////////Generating job from Function\n";
	ArgumentVector v11;
	v11.push_back( 10 );
	FunctionObject *f11 = Bind( &SimpleFunction11 );
	//
	Job *j = new JobFromFunction( f11, v11 );
	JobSystem::GetSingleton().Add(j);
}