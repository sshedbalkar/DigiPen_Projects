// JobSystem.cpp : Defines the entry point for the console application.
//

#include "JobSystem.hpp"
JobSystem* gJobs;

void StartJobSystem()
{
  gJobs = new JobSystem();
  gJobs->Startup();
}

void JobWorker::Startup(uint index)
{
	thread = new Thread();
	char name[100];
	sprintf(name, "Worker %d", index);
	thread->Initialize( Thread::ObjectEntryCreator<JobWorker, &JobWorker::Main> ,this, name);
	thread->Resume();
	CurrentJob = NULL;
}

OsInt JobWorker::Main()
{
	JobSystem* jobs = gJobs;

	while(jobs->mActive)
	{
		jobs->JobCounter.Decrement();

		Job* job = jobs->GetNextJob();
		if(job)
		{
			//Do the job
			job->Execute();

			if(job->DeleteOnCompletion)
				delete job;
		}
	}

	return 0;

}

Job* JobSystem::GetNextJob()
{
	StackThreadLock lock(&JobLock);

	if(PendingJobs.empty())
		return NULL;

	Job* nextJob = PendingJobs.front();
	PendingJobs.erase( PendingJobs.begin() );

	return nextJob;
}

void JobSystem::Add(Job* job)
{
	StackThreadLock lock(&JobLock);

	PendingJobs.push_back(job);

	JobCounter.Increment();
}

void JobSystem::Shutdown()
{
	mActive = false;


	
	//un block all
	for(uint i=0;i<Workers.size();++i)
		JobCounter.Increment();


	for(uint i=0;i<Workers.size();++i)
		Workers[i].thread->WaitForCompletion();


}

void JobSystem::Startup()
{
	mActive = true;

	SYSTEM_INFO sysinfo; 
	GetSystemInfo( &sysinfo );  
	uint numberOfCores = sysinfo.dwNumberOfProcessors; 

	Workers.resize(numberOfCores);
	for(uint i=0;i<Workers.size();++i)
		Workers[i].Startup(i);
}