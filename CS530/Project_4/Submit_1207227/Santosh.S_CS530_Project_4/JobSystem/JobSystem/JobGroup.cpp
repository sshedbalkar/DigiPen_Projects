#include "JobGroup.h"
#include <iostream>
//
JobGroup::JobGroup()
{

}
JobGroup::~JobGroup()
{

}
void JobGroup::Execute()
{
	Status = Running;
	std::cout<<"Started Group Job\n";
	WaitForChildJobs();
	
	std::cout<<"Executed Group Job\n";
	Status = Completed;
}
void JobGroup::WaitForChildJobs()
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
void JobGroup::AddChildJob( Job* job )
{
	m_childJobs.push_back( job );
	job->DeleteOnCompletion = false;
}