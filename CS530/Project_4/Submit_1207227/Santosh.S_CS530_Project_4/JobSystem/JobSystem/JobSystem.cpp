// JobSystem.cpp : Defines the entry point for the console application.
//
#include "JobSystem.hpp"
//
//bool JobWorker::m_wasEmpty = true;
//
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
	JobSystem* jobs = &JobSystem::GetSingleton();

	while(jobs->mActive)
	{
		jobs->JobCounter.Decrement();

		Job* job = jobs->GetNextJob();
		if(job)
		{
			jobs->AddToActiveJobs( job );
			//Do the job
			//std::cout<<"Executing job with priority: "<<job->Priority<<"\n";
			job->Execute();
			jobs->RemoveFromActiveJobs( job );

			if(job->DeleteOnCompletion)
				delete job;
			//else
				//jobs->AddToCompletedJobs( job );
		}
	}
	//
	return 0;
}
JobSystem::JobSystem()
{
	//
}
JobSystem::~JobSystem()
{
	//
}
Job* JobSystem::GetNextJob()
{
	StackThreadLock lock(&JobLock);

	if(PendingJobs.empty())
		return NULL;

	Job* nextJob = PendingJobs.front();
	//std::cout<<"Removed job: "<<nextJob->Priority<<"\n";
	PendingJobs.erase( PendingJobs.begin() );

	return nextJob;
}

void JobSystem::Add(Job* job)
{
	StackThreadLock lock(&JobLock);

	//PendingJobs.push_back(job);
	//std::cout<<"Adding job: "<<job->Priority<<"\n";
	bool added = false;
	int i = 0;
	for( std::vector<Job*>::iterator it1 = PendingJobs.begin(), it2 = PendingJobs.end(); it1 != it2; ++it1 )
	{
		if( (*it1)->Priority < job->Priority )
		{
			//std::cout<<i<<": "<<(*it1)->Priority<<": "<<job->Priority<<"\n";
			PendingJobs.insert( it1, job );
			added = true;
			break;
		}
	}
	if( !added )
	{
		PendingJobs.push_back(job);
	}
	JobCounter.Increment();
}

void JobSystem::AddToCompletedJobs(Job* job)
{
	StackThreadLock lock(&JobLock);

	CompeletedJobs.push_back(job);

	JobCounter.Increment();
}
void JobSystem::AddToActiveJobs(Job* job)
{
	StackThreadLock lock(&JobLock);

	ActiveJobs.push_back(job);

	JobCounter.Increment();
}
void JobSystem::RemoveFromActiveJobs( Job *job )
{
	StackThreadLock lock(&JobLock);

	for( std::vector<Job*>::iterator it1 = ActiveJobs.begin(), it2 = ActiveJobs.end(); it1 != it2; ++it1 )
	{
		if( (*it1) == job )
		{
			//std::cout<<"Removing job: "<<(*it1)->Priority<<"\n";
			ActiveJobs.erase( it1 );
			break;
		}
	}

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