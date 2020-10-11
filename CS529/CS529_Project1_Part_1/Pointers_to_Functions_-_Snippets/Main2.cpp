// ---------------------------------------------------------------------------
// Project Name		:	Snippet 2
// File Name		:	Main.cpp
// Author			:	Karim Fikani
// Creation Date	:	12/02/2009
// Purpose			:	Example on how to use an array of function pointers
// History			:
// ---------------------------------------------------------------------------


#include <stdio.h>

void Foo1(void);
void Foo2(void);
void Foo3(void);
void Foo4(void);
void Foo5(void);

int main()
{
	// Creating an array of function pointers
	void (*arrayFP[])(void) = {Foo1, Foo2, Foo3, Foo4, Foo5};

	arrayFP[0]();
	(*arrayFP[1])();

	return 0;
}

void Foo1(void)
{
	printf("Foo1\n");
}

void Foo2(void)
{
	printf("Foo2\n");
}

void Foo3(void)
{
	printf("Foo3\n");
}

void Foo4(void)
{
	printf("Foo4\n");
}

void Foo5(void)
{
	printf("Foo5\n");
}