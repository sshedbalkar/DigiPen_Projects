#pragma once
//
#include "JobSystem.hpp"
#include <vector>
//
class JobGroup: public Job
{
public:
	JobGroup();
	~JobGroup();
	void Execute();
	void AddChildJob( Job* job );
	//
private:
	void WaitForChildJobs();
	//
private:
	std::vector< Job* > m_childJobs;
	//
};