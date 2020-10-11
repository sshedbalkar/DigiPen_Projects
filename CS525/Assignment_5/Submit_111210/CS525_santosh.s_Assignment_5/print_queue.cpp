#include "print_queue.h"

////////////////////////////////////////////////////////////
/* PrintQueue implementation */
////////////////////////////////////////////////////////////

/* constructor 
 * pointer to a simulator that's running it
 */ 
PrintQueue::PrintQueue(Simulator* _psim) : 
	psim(_psim),num_available_printers(0) 
{}

/* to use a printer we need to register it with the print queue first */
void PrintQueue::RegisterPrinter(int ID, double speed) {
	/* pair of (ID and a pair (available, speed) ) */
//	printer_status.insert( 
//			std::make_pair<int,std::pair<bool,double> >
//			         (ID,std::make_pair<bool,double>(true,speed) ) 
//			);
//next line is equivalent to the above insert
	printer_status[ ID ] = std::make_pair<bool,double>(true,speed);
	//printer is available by default
	++num_available_printers;
}

/* implements 2) from PrintQueue */
void PrintQueue::JobFinished( double current_time, int printerID, PrintJob job)
{
	ID2pair::iterator it = printer_status.find( printerID );

	if ( it == printer_status.end() ) throw "invalid printerID";

	std::cout << "Printer " << printerID << " finished job " 
		<< job.jobID << " at time " << current_time << std::endl;


	if ( !jobs_in_queue.empty() ) { //immediately assign another job
		//...
		///< Get the topmost job and create a PrintJob object and assign the object to the printer simulator
		PrintJob new_job(jobs_in_queue.top().first);
		psim->AddEvent(new EventJobFinished (current_time+new_job.size/printer_status.find(printerID)->second.second, new_job,printerID, this, &PrintQueue::JobFinished));
		///< decrement the queue by 1
		jobs_in_queue.pop();

		std::cout << "Printer " << printerID << " assigned new job " 
			<< new_job.jobID << " at time " << current_time << std::endl;

	} else { // no jobs in the queue - printer rests
		++num_available_printers;
		it->second.first = true;
	}
}

/* implements 3) from PrintQueue */
void PrintQueue::NewJobArrived( double current_time, PrintJob job )
{
	std::cout << "New job " << job.jobID 
		<< " at time " << current_time << std::endl;

	if ( num_available_printers>0 ) { //self-explanatory
		//find an available printer
		ID2pair::iterator it = printer_status.begin();
		while ( it != printer_status.end() && it->second.first==false ) {++it;}

		//if condition holds, we have a problem with data: 
		//num_available_printers>0 but no printer seem to be available
		if (it==printer_status.end()) throw "corrupted data";

		//...
		///< Assign new job to the printer and update states
		it->second.first=false;
		--num_available_printers;
		///< Create a new Job Object from the job to be executed and assign it to printer simulator
		psim->AddEvent(new EventJobFinished(current_time + job.size/it->second.second, job, it->first, this, &PrintQueue::JobFinished));
		std::cout << "Job assigned to printer " << it->first
			<< " at time " << current_time << std::endl;
	} else { 
		std::cout << "No available printers - put in print queue\n";
		//...
		std::pair<PrintJob, double> pr = std::make_pair<PrintJob, double>(job, current_time);
		jobs_in_queue.push(pr);
	}
}

////////////////////////////////////////////////////////////
/* EventJobFinished implementation */
////////////////////////////////////////////////////////////
EventJobFinished::EventJobFinished(double when, const PrintJob& _job, int _printer_id,PrintQueue* _p_print_queue, void(PrintQueue::*_p_method)(double, int, PrintJob)):job(_job), printer_id(_printer_id), p_print_queue(_p_print_queue), p_method(_p_method){
	SetWhen(when);
}

///< Execute the EventJobFinished method
void EventJobFinished::Execute(){
	(p_print_queue->*p_method)(this->When(),printer_id,job);
}

////////////////////////////////////////////////////////////
/* EventNewJobArrived implementation */
////////////////////////////////////////////////////////////

EventNewJobArrived::EventNewJobArrived(double when, const PrintJob& _job, PrintQueue* _p_print_queue, void (PrintQueue::*_p_method)(double, PrintJob)):job(_job), p_print_queue(_p_print_queue), p_method(_p_method)
{
	SetWhen(when);
}

///< Execute the EventNewJobArrived method
void EventNewJobArrived::Execute(){
	(p_print_queue->*p_method)(this->When(), job);
}
