// ---------------------------------------------------------------------------
// Project Name		:	Snippet 4
// File Name		:	Main.cpp
// Author			:	Karim Fikani
// Creation Date	:	12/02/2009
// Purpose			:	Example on how to use function pointers as parameters
// History			:
// ---------------------------------------------------------------------------

#include <stdio.h>


float Calculations(float a, float b, float (*pf)(float, float));

float Plus(float a, float b);
float Minus(float a, float b);
float Multiply(float a, float b);
float Divide(float a, float b);



int main()
{
	printf("Result: %f\n", Calculations(3, 5, Minus));

	return 0;
}


float Calculations(float a, float b, float (*pf)(float, float))
{
	float result = pf(a, b);
	return result;
}


inline float Plus(float a, float b)
{
	return a+b;
}

inline float Minus(float a, float b)
{
	return a-b;
}

inline float Multiply(float a, float b)
{
	return a*b;
}

inline float Divide(float a, float b)
{
	return a/b;
}