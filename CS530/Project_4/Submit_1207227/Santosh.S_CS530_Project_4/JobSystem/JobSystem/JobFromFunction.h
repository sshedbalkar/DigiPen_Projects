#pragma once
//
#include "JobSystem.hpp"
#include "common\FunctionBind.h"
//
class JobFromFunction: public Job
{
public:
	JobFromFunction( FunctionObject *func, ArgumentVector &vec )
		:m_func( func),
		m_args( vec )
	{
		//
	}

	void Execute()
	{
		Status = Running;
		//do something
		m_func->Invoke( NULL, m_args );
		printf("Test Job2 completed\n");
		Status = Completed;
	}
	//
private:
	FunctionObject *m_func;
	ArgumentVector m_args;
};