#include "JobSystem.hpp"
#include "Tester.h"
#include "TestJob1.h"
//
int main(int argc, char* argv[])
{
	JobSystem *js = new JobSystem();
	JobSystem::GetSingleton().Startup();
	//StartJobSystem();
	
	printf("Jobs System Ready\n");
	//
	Tester t;
	t.Test();
	//
	
	while(1)
	{
		int next = getchar();

		if(next == 'x')
			break;

		if(next == 'b')
		{
			Job* j = new TestJob1(1);
			JobSystem::GetSingleton().Add(j);
		}

		if(next == 'r')
		{
			for(uint i=0;i<8000;++i)
			{
				Job* j = new TestJob1( rand() % 3 );
				JobSystem::GetSingleton().Add(j);
			}
		}
	}
	
	JobSystem::GetSingleton().Shutdown();
	std::cout<<"Completed\n";
	//
	delete js;
	return 0;
}

