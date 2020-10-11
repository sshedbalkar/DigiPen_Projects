// ---------------------------------------------------------------------------
// Project Name		:	Snippet 5
// File Name		:	Main.cpp
// Author			:	Karim Fikani
// Creation Date	:	12/02/2009
// Purpose			:	Example on how to return a function pointer
// History			:
// ---------------------------------------------------------------------------

#include <stdio.h>

typedef float (*pf)(float , float);

float Calculations(float a, float b, float (*pf)(float, float));

float Plus(float a, float b);
float Minus(float a, float b);
float Multiply(float a, float b);
float Divide(float a, float b);

pf GetSign(const char sign);


int main()
{

	printf("Result: %f\n", Calculations(3, 5, GetSign('+')));

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

pf GetSign(const char sign)
{
	switch(sign)
	{
	case '+':
		return Plus;

	case '-':
		return Minus;

	case '*':
		return Multiply;

	case '/':
		return Divide;

	default:
		return 0;
	}
}