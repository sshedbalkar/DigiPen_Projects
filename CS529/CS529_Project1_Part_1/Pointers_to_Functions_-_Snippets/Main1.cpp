// ---------------------------------------------------------------------------
// Project Name		:	Snippet 1
// File Name		:	Main.cpp
// Author			:	Karim Fikani
// Creation Date	:	12/02/2009
// Purpose			:	Example on how to use function pointers
// History			:
// ---------------------------------------------------------------------------


#include <stdio.h>

void	Foo(void);
float	Boo(float a, float b);

int main()
{
	//----------------------------------------
	//Declaring a function pointer to Foo()
	
	void (*pfFoo)(void) = 0;
	pfFoo = Foo;
	//pfFoo = &Foo; // Also applicable

	pfFoo();
	(*pfFoo)(); // Also applicable


	//----------------------------------------
	//Declaring a function pointer to Boo()

	float (*pfBoo)(float, float) = 0;
	pfBoo = Boo;
	//pfBoo = &Boo; // Also applicable

	printf("Result: %f\n", pfBoo(3, 4));
	printf("Result: %f\n", (*pfBoo)(3, 4)); // Also applicable

	return 0;
}


void	Foo(void)
{
	printf("Foo\n");
}

float	Boo(float a, float b)
{
	return a+b;
}