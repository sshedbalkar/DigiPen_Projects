#pragma once
//
class Tester
{
public:
	Tester();
	~Tester();
	//
	void Test();
	//
private:
	void StressTest();
	void GenerateJobFromFunc();
	void PriorityTest();
	void ComplexJobTest();
	void JobGroupTest();
};