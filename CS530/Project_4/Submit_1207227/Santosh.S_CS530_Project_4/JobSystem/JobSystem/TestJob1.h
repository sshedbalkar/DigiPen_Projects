#pragma once
//
#include "JobSystem.hpp"
//
class TestJob1: public Job
{
public:
	TestJob1(int t)
	{
		d = t;
	}

	int d;

	void Execute()
	{
		//do something
		Status = Running;

		float total = 0.0f;

		for(uint i=0;i<1000 * d;++i)
		{
			total += cosf( float(i) );
		}

		printf("Test Job1  %f\n", total);
		Status = Completed;
	}
};