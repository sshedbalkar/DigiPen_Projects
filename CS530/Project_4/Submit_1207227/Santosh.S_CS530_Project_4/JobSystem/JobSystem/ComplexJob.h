#pragma once
//
#include "JobSystem.hpp"
#include <vector>
//
class ComplexJob: public Job
{
public:
	ComplexJob();
	~ComplexJob();
	void Execute();
	void AddChildJob( Job* job );
	virtual void ExecuteDependentJob();
	//
private:
	void WaitForChildJobs();
	//
private:
	std::vector< Job* > m_childJobs;
	//
};