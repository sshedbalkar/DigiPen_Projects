#include <iostream>

using namespace std;

//(int *) (*)[5] foo(int)
//int* [5] foo(int);
//int (*f2(int))[5]
//int* (*foo(int))[5]
//int* f1(int)
//{
//
//}
//int* (*foo[5])(int)

int(*foo(int (*)(int)))[5]
int main(int argc, char* argv[])
{
	signed int var1 = -1024;

	var1 >>= 2;
	cout << "Value: " << var1;

	

	cin.get();
}