#include <iostream>
#include <cmath>
#include <time.h>
//
struct Integral
{
	double val;
	int iter;
};
//
Integral compositeIntegration(int m, int id, double a, double b, double e);
double integrate(int m, int id, double a, double b, int n);
double integration_Trapezoidal(int id, double a, double b, int n);
double integration_Simpsons(int id, double a, double b, int n);
Integral integration_MonteCarlo(int id, double a, double b, double e);
double integration_MidPoint(int id, double a, double b, int n);
double funcValue(int id, double x);
double randomDouble(double min, double max);
//
int main()
{
	int m_func = 0;
	double m_a, m_b, m_esp;
	Integral i;
	char m_choice;
	//
	srand((unsigned)time(NULL));
	//
	std::cout<<"################################################################################\n";
	std::cout<<"MAT557 Project 1: Calculation of Definite Integrals using Composite Quadrature Techniques\n";
	std::cout<<"by Santosh Shedbalkar|MSCS|60001111\n";
	std::cout<<"################################################################################\n";
	//
	do
	{
		std::cout<<"-----------------------------------------------------\n";
		std::cout<<"\nFunctions:\n";
		std::cout<<"1. (2*x)*(sin(x)+2)\n";
		std::cout<<"2. (cos^2(x)+5)*ln(x)\n";
		std::cout<<"3. (sin(x)+2)/ln(2*x)\n";
		std::cout<<"\nEnter your function choice: (1/2/3): ";
		std::cin>>m_func;
		std::cout<<"Your Choice: "<<m_func<<"\n";
		//
		std::cout<<"\nEnter the starting Range(a): ";
		std::cin>>m_a;
		std::cout<<"You entered: a = "<<m_a<<"\n";
		//
		std::cout<<"\nEnter the end Range(b): ";
		std::cin>>m_b;
		std::cout<<"You entered: b = "<<m_b<<"\n";
		//
		std::cout<<"\nEnter the acceptable error value(Epsilon): ";
		std::cin>>m_esp;
		std::cout<<"You entered: Epsilon = "<<m_esp<<"\n";
		//
		std::cout<<"\n*****Result*****\n";
		i = compositeIntegration(0, m_func, m_a, m_b, m_esp);
		std::cout<<"Midpoint Rule:			Value: "<<i.val<<",		Iterations: "<<i.iter<<"\n";
		i = compositeIntegration(1, m_func, m_a, m_b, m_esp);
		std::cout<<"Trapezoidal Rule:		Value: "<<i.val<<",		Iterations: "<<i.iter<<"\n";
		i = compositeIntegration(2, m_func, m_a, m_b, m_esp);
		std::cout<<"Simpson's Rule:			Value: "<<i.val<<",		Iterations: "<<i.iter<<"\n";
		i = compositeIntegration(3, m_func, m_a, m_b, m_esp);
		std::cout<<"Monte-Carlo Method:		Value: "<<i.val<<",		Iterations: "<<i.iter<<"\n";
		//
		std::cout<<"\nDo you want to continue (y/n)?: ";
		std::cin>>m_choice;
	}
	while(m_choice == 'y' || m_choice == 'Y');
	//
	std::cin.get();
}
Integral compositeIntegration(int m, int id, double a, double b, double e)
{
	Integral inte;
	if(m != 3)
	{
		int i = 1, max = 100000;
		double Sn = 0;
		double Sn1 = 10000000.0;
		//
		while((abs(Sn - Sn1) > e) && (i < max))
		{
			Sn = integrate(m, id, a, b, i);
			Sn1 = integrate(m, id, a, b, i + 1);
			inte.iter = i;
			inte.val = Sn;
			++i;
		}
	}
	else
	{
		inte = integration_MonteCarlo(id, a, b, e);
	}
	//
	return inte;
}
double integrate(int m, int id, double a, double b, int n)
{
	if(m == 0)
	{
		return integration_MidPoint(id, a, b, n);
	}
	else if(m == 1)
	{
		return integration_Trapezoidal(id, a, b, n);
	}
	else if(m == 2)
	{
		return integration_Simpsons(id, a, b, n);
	}
	else if(m == 3)
	{
		//return integration_MonteCarlo(id, a, b, n);
	}
	else 
	{
		return 0.0;
	}
}
double integration_Trapezoidal(int id, double a, double b, int n)
{
	double h = (b - a) / n;
	double val = 0.0;
	double xk;
	//
	for(int i = 1; i < n; ++i)
	{
		xk = a + (i * h);
		val += funcValue(id, xk);
	}
	val *= 2;
	val += funcValue(id, a);
	val += funcValue(id, b);
	val *= h;
	val /= 2;
	//
	return val;
}
double integration_Simpsons(int id, double a, double b, int n)
{
	double h = (b - a) / n;
	double val = 0.0, temp1 = 0.0;
	double xk;
	//
	for(int i = 1; i < n; ++i)
	{
		xk = a + (i * h);
		temp1 += funcValue(id, xk);
	}
	val += 2 * temp1;
	//
	temp1 = 0.0;
	for(int i = 0; i < n; ++i)
	{
		xk = a + (i * h);
		xk *= 2;
		xk += h;
		xk /= 2;
		temp1 += funcValue(id, xk);
	}
	temp1 *= 4;
	val += temp1;
	val += funcValue(id, a);
	val += funcValue(id, b);
	val *= h;
	val /= 6;
	//
	return val;
}
Integral integration_MonteCarlo(int id, double a, double b, double e)
{
	Integral inte;
	double nTop = 0;
	double x, y;
	double c;
	double Sn, Sn1 = 0.0;
	int n = 50, max = 1000000;
	//
	if(funcValue(id, a) < funcValue(id, b))
	{
		c = ceil(funcValue(id, b) + 1);
	}
	else
	{
		c = ceil(funcValue(id, a) + 1);
	}
	//
	for(int i = 0; i < n; ++i)
	{
		x = randomDouble(a, b);
		y = randomDouble(0.0, c);
		if(y <= funcValue(id, x))
		{
			++nTop;
		}
	}
	do
	{
		++n;
		x = randomDouble(a, b);
		y = randomDouble(0.0, c);
		Sn = Sn1;
		Sn1 = (nTop/n)*(b - a) * c;
		if(y <= funcValue(id, x))
		{
			++nTop;
		}
	}
	while((abs(Sn - Sn1) > e) && (n < max));
	//
	inte.iter = n;
	inte.val = Sn;
	return inte;
}
double integration_MidPoint(int id, double a, double b, int n)
{
	double h = (b - a) / n;
	double val = 0.0;
	double xk;
	//
	for(int i = 0; i < n; ++i)
	{
		xk = a + (i * h);
		xk *= 2;
		xk += h;
		xk /= 2;
		val += funcValue(id, xk);
	}
	val *= h;
	//
	return val;
}
double funcValue(int id, double x)
{
	double val = 0.0;
	if(id == 1)
	{
		val = ((2 * x) * (sin(x) + 2));
	}
	else if(id == 2)
	{
		val = ((cos(x) * cos(x)) + 5) * log(x);
	}
	else if(id == 3)
	{
		val = (sin(x)+2) / log(2 * x);
	}
	//
	return val;
}
double randomDouble(double min, double max)
{
	double r = (double)rand() / (double)RAND_MAX;
	return min + r * (max - min);
}