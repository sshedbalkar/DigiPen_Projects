#ifndef GENERATE_DATA_H
#define GENERATE_DATA_H
#include <vector>
#include <string>
#include <map>
#include <iostream>

/*
 * Simple Bayesian Network class.
 *
 * implements:
 * network structure (topology) 
 * Conditional Probability Tables
 * nodes (random variables) are enumerated 0..size-1
 *
 * features:
 * automatic native ordering (all parent are in front of child)
 * sample generation - may be used to either generate training data 
 * or for inference (calculating probabilities) by sampling
 *
 * TODO:
 * 1) exact inference
 * 2) topology discovery
 * 3) network testing (given data, calculate 
 * 4) Markov sampling
 * 5) topology test - CheckForLoops
 *
 * Dmitri Volper
 * Fri 28 Mar 2008 10:51:52 AM PDT
 */

class TrainingData {
	public:
		TrainingData(std::string filename);
		TrainingData(unsigned int size);
		unsigned int Size() const;
		void AddRecord(const std::vector<bool> & record);
		double EstimateProbability( const std::string & query);
		void Learn();
		//
	private:
		unsigned int BN_size;
		std::vector<std::vector<bool> > dataset;
		std::map<std::string,double> cache;

		bool Match(const std::vector<bool> & record,
				const std::vector<unsigned char> & pattern) const;
};

class BayesianNetwork {
	public:
		BayesianNetwork(unsigned int size);

		//creates parent relation
		void AddLink(unsigned int parent, unsigned int child);

		//add CPT table to a node
		void AddCPT(unsigned int nodeID, std::vector<double> cpt);
		//a couple overloads for convenience
		void AddCPT(unsigned int nodeID, double p);
		void AddCPT(unsigned int nodeID, double p0, double p1);
		void AddCPT(unsigned int nodeID, double p0, double p1, double p2, double p3);

		//samples from the joint distribution specified number of times
		void GenerateData(unsigned int datasize,TrainingData & td);
		void GenerateData(unsigned int datasize,std::string filename);

		void Print () const;
		//probability by sampling
		//double EstimateProbability( const std::string & query);

		unsigned int Size() const { return BN_size; }
		//seed RNG to simplify debugging - see main.cpp
		void SeedRNG( unsigned int seed) const { std::srand(seed); }

		//couple of tests to make sure things work the way I think they should
		void Tests(double success_rate, const std::vector<bool> & b) const {
			int num_T =0;
			for (int i=0;i<10000;++i) {
				if ( RandomBool( success_rate ) ) ++num_T;
			}
			std::cout << num_T << " at " << success_rate << std::endl;

			for (std::vector<bool>::const_iterator it = b.begin();
					it!=b.end();
					++it) std::cout << *it;
			std::cout << " = " << bin2dec(b) << std::endl;
		}
		//
		double ExactInference( const std::string& query);
		//
	protected:
		//auxiliary functions
		//return boolean with P(true)=probability
		bool RandomBool( double probability) const;
		//binary to decimal conversion
		//vector {0,1,1,1,0,1} -> 1+4+8+16 = 29
		unsigned int bin2dec(const std::vector<bool> & b) const;
		bool NativeOrder();
		template <typename T>
		void GenerateData_aux(unsigned int datasize, T inserter);
		//
		typedef std::vector<unsigned char> EventType;
		typedef std::vector<EventType> EventListType;
		typedef std::vector<bool> BoolList;
		//
		double SumOfMatchingEvents( const EventType& distribution );
		double ProbOfEvent( const EventType& evnt );
		void GetAllEventsForTheDistribution( const EventType& distribution, EventListType& result );
		double GetProbOfAtomicEvent( unsigned evnt, const EventType& parentsStatus );
		void GetStatusofParents( unsigned child, const EventType& evnt, BoolList& parnts );
		//
	private:
		unsigned int BN_size;
		bool topologyDone;

		//ordering where for each node all its parents have smaller indicies
		//for example, all nodes without parents are in front
		std::vector<unsigned int> nativeOrdering; 

		//evident
		std::vector<std::vector<unsigned int> > parents;

		//CPT for each node, each CPT is ordered naturally (see comment for AddCPT
		//size of CPT for node i is
		//2^parents[i].size()
		std::vector<std::vector<double> > CPTs;
};

#endif

