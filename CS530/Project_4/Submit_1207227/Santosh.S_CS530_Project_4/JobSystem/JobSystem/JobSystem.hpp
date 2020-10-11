#pragma once
//
#include "Os.hpp"
#include <vector>
#include <unordered_map>
#include "Thread.hpp"
#include <queue>
#include "common\Singleton.h"

enum JobStatus
{
	Waiting,
	Running,
	Completed
};

class Job
{
public:
	Job()
	{
		DeleteOnCompletion = true;
		Status = Waiting;
		Priority = 0;
	}
	unsigned int Priority;
	bool DeleteOnCompletion;
	JobStatus Status;

	virtual ~Job(){}
	virtual void Execute()=0;
};


class JobWorker
{
public:
	void Startup(uint index);
	OsInt Main();
	Job* CurrentJob;
	Thread* thread;
};

class JobSystem: public Singleton< JobSystem >
{
public:
	JobSystem();
	~JobSystem();
	//
	bool mActive;

	std::vector<JobWorker> Workers;

	std::vector<Job*> PendingJobs;
	std::vector<Job*> ActiveJobs;
	std::vector<Job*> CompeletedJobs;

	void RunCompletions();
	void Startup();
	void WaitForAll();
	void Add(Job* job);
	void Shutdown();
	void AddToActiveJobs( Job *job );
	void AddToCompletedJobs( Job *job );
	void RemoveFromActiveJobs( Job *job );


	Job* GetNextJob();

	Semaphore JobCounter;
	ThreadLock JobLock;
};

extern JobSystem* gJobs;

void StartJobSystem();


