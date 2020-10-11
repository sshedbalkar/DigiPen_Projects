// ---------------------------------------------------------------------------
// Project Name		:	Snippet 3
// File Name		:	Main.cpp
// Author			:	Karim Fikani
// Creation Date	:	12/02/2009
// Purpose			:	Example on how to give another name to the function
//						pointer using typedef
// History			:
// ---------------------------------------------------------------------------

#include <stdio.h>

typedef void (*FP)(int);

void Foo1(int a);
void Foo2(int a);
void Foo3(int a);
void Foo4(int a);
void Foo5(int a);

int main()
{
	// Creating an array of function pointers
	//void (*arrayFP[])(int) = {Foo1, Foo2, Foo3, Foo4, Foo5};
	// or
	// using typedef
	FP	arrayFP[5] = {Foo1, Foo2, Foo3, Foo4, Foo5};

	arrayFP[0](1);
	(*arrayFP[1])(1);

	return 0;
}

void Foo1(int a)
{
	printf("Foo1: %i\n", a*10);
}

void Foo2(int a)
{
	printf("Foo2: %i\n", a*20);
}

void Foo3(int a)
{
	printf("Foo3: %i\n", a*30);
}

void Foo4(int a)
{
	printf("Foo4: %i\n", a*40);
}

void Foo5(int a)
{
	printf("Foo5: %i\n", a*50);
}