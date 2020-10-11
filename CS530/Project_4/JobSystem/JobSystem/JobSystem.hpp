#include "Os.hpp"

#include <vector>
#include <unordered_map>
#include "Thread.hpp"
#include <queue>

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
  }

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

class JobSystem
{
public:
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


  Job* GetNextJob();

  Semaphore JobCounter;
  ThreadLock JobLock;
};

extern JobSystem* gJobs;

void StartJobSystem();


