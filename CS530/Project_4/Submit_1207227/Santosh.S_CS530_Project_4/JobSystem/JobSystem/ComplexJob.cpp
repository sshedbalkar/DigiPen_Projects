#include "ComplexJob.h"
#include <iostream>
//
ComplexJob::ComplexJob()
{

}
ComplexJob::~ComplexJob()
{

}
void ComplexJob::ExecuteDependentJob()
{
	float total = 0.0f;
	for(uint i=0;i<5000; ++i)
	{
		total += cosf( float(i) );
	}
	//
	std::cout<<"Executed Complex Job\n";
}
void ComplexJob::Execute()
{
	Status = Running;
	std::cout<<"Started Complex Job\n";
	WaitForChildJobs();
	//
	ExecuteDependentJob();
	Status = Completed;
}
void ComplexJob::WaitForChildJobs()
{
	Job *j;
	while( !m_childJobs.empty() )
	{
		for( std::vector< Job* >::iterator it1 = m_childJobs.begin(), it2 = m_childJobs.end(); it1 != it2; ++it1 )
		{
			if( ( j = (*it1))->Status == Completed )
			{
				m_childJobs.erase( it1 );
				delete j;
				break;
			}
		}
		Sleep(1);
	}
}
void ComplexJob::AddChildJob( Job* job )
{
	m_childJobs.push_back( job );
	job->DeleteOnCompletion = false;
}