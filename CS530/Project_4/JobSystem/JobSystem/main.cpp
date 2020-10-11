#include <stdio.h>

#include "JobSystem.hpp"

class TestJob : public Job
{
public:
	TestJob(int t)
	{
		d = t;
	}

	int d;

	void Execute()
	{
		//do something


		float total = 0.0f;

		for(uint i=0;i<1000 * d;++i)
		{
			total += cosf( float(i) );
		}

		printf("Test Job  %f\n", total);
	}
};

int main(int argc, char* argv[])
{

  StartJobSystem();

  printf("Jobs System Ready\n");

  while(1)
  {
	  int next = getchar();

	  if(next == 'x')
		  break;

	  if(next == 'b')
	  {
		  Job* j = new TestJob(1);
		  gJobs->Add(j);
	  }

	  if(next == 'r')
	  {
		  for(uint i=0;i<8000;++i)
		  {
			  Job* j = new TestJob( rand() % 3 );
			  gJobs->Add(j);
		  }
	  }

  }

  gJobs->Shutdown();

  return 0;
}

