#include <iostream>
#include <cmath>
#include <vector>
#include "koolplot.h"

double f1(double t)
{
	double temp=4.0/3.0;
	temp+= 0.5*sin(2*t);
	temp-= (1.0/3.0)*cos(3*t);
	return temp;
}

double f1prime(double t,double y)
{
	return (cos(2.0*t)+sin(3.0*t));
}

void Euler(std::vector<double> &t,std::vector<double> &w,double t0,double tn,double w0,int N,double (*f)(double,double))
{
	double h=(tn-t0)/(double)N;
	t.clear();
	w.clear();

	double t_temp=t0;
	double w_temp=w0;

	t.push_back(t_temp);
	w.push_back(w_temp);

	for(int i=1;i<=N;i++)
	{
		w_temp+= (h*f(t_temp,w_temp));
		t_temp=t0+(double)i*h;

		t.push_back(t_temp);
		w.push_back(w_temp);
	}
}

void ModifiedEuler(std::vector<double> &t,std::vector<double> &w,double t0,double tn,double w0,int N,double (*f)(double,double))
{
	double h=(tn-t0)/(double)N;
	t.clear();
	w.clear();

	double t_temp=t0;
	double w_temp=w0;

	t.push_back(t_temp);
	w.push_back(w_temp);

	for(int i=1;i<=N;i++)
	{
		w_temp+= (h/2.0)* (f(t_temp,w_temp)+f(t_temp+h,w_temp + h*f(t_temp,w_temp)));
		t_temp= t0+(double)i*h;

		t.push_back(t_temp);
		w.push_back(w_temp);
	}
}

void RungeKutta(std::vector<double> &t,std::vector<double> &w,double t0,double tn,double w0,int N,double (*f)(double,double))
{
	double h=(tn-t0)/(double)N;
	t.clear();
	w.clear();

	double t_temp=t0;
	double w_temp=w0;
	double K1,K2,K3,K4;
	t.push_back(t_temp);
	w.push_back(w_temp);

	for(int i=1;i<=N;i++)
	{
		K1 = h*f(t_temp,w_temp);
		K2 = h*f(t_temp+(h/2.0),w_temp+(K1/2));
		K3 = h*f(t_temp+(h/2.0),w_temp+(K2/2));
		K4 = h*f(t_temp+h,w_temp+K3);
		w_temp+= (K1+2.0*K2+2.0*K3+K4)/6.0;
		t_temp= t0+(double)i*h;

		t.push_back(t_temp);
		w.push_back(w_temp);
	}
}

double function1(double t,double w1,double w2)
{
	return w2;
}

double function2(double t,double w1,double w2)
{
	double temp;
	temp = (exp(2.0*t) )*sin(t);
	temp -= 2.0*w1;
	temp += 2.0*w2;
	return temp;
}

double function11(double t,double w1,double w2)
{
	return w2;
}

double function22(double t,double w1,double w2)
{
	double temp;
	temp = (exp(t))*t;
	temp -= t;
	temp += 2.0*w2;
	temp -= w1;
	return temp;
}

void RungeKutta(std::vector<double> &t,std::vector<double> &w1,std::vector<double> &w2,double t0,double tn,
	double initial_w1,double initial_w2,int N,double (*f1)(double,double,double),double (*f2)(double,double,double))
{
	double h=(tn-t0)/(double)N;
	t.clear();
	w1.clear();
	w2.clear();

	double t_temp=t0;
	double w1_temp=initial_w1;
	double w2_temp=initial_w2;
	double K11,K21,K31,K41;
	double K12,K22,K32,K42;

	t.push_back(t_temp);
	w1.push_back(w1_temp);
	w2.push_back(w2_temp);

	for(int i=1;i<=N;i++)
	{
		K11 = h*f1(t_temp,w1_temp,w2_temp);
		K12 = h*f2(t_temp,w1_temp,w2_temp);
		
		K21 = h*f1(t_temp+(h/2.0),w1_temp+(K11/2.0),w2_temp+(K12/2.0));
		K22 = h*f2(t_temp+(h/2.0),w1_temp+(K11/2.0),w2_temp+(K12/2.0));

		K31 = h*f1(t_temp+(h/2.0),w1_temp+(K21/2.0),w2_temp+(K22/2.0));
		K32 = h*f2(t_temp+(h/2.0),w1_temp+(K21/2.0),w2_temp+(K22/2.0));

		K41 = h*f1(t_temp+h,w1_temp+K31,w2_temp+K32);
		K42 = h*f2(t_temp+h,w1_temp+K31,w2_temp+K32);

		w1_temp += (K11+2.0*K21+2.0*K31+K41)/6.0;
		w2_temp += (K12+2.0*K22+2.0*K32+K42)/6.0;

		t_temp= t0+(double)i*h;

		t.push_back(t_temp);
		w1.push_back(w1_temp);
		w2.push_back(w2_temp);
	}
}

void Euler(std::vector<double> &t,std::vector<double> &w1,std::vector<double> &w2,double t0,double tn,
	double initial_w1,double initial_w2,int N,double (*f1)(double,double,double),double (*f2)(double,double,double))
{
	double h=(tn-t0)/(double)N;
	t.clear();
	w1.clear();
	w2.clear();

	double t_temp=t0;
	double w1_temp=initial_w1;
	double w2_temp=initial_w2;
	
	t.push_back(t_temp);
	w1.push_back(w1_temp);
	w2.push_back(w2_temp);

	for(int i=1;i<=N;i++)
	{
		double a,b;
		a=w1_temp;
		b=w2_temp;

		w1_temp+= (h*f1(t_temp,a,b));
		w2_temp+= (h*f2(t_temp,a,b));
		
		t_temp=t0+(double)i*h;

		t.push_back(t_temp);
		w1.push_back(w1_temp);
		w2.push_back(w2_temp);
	}
}

void ModifiedEuler(std::vector<double> &t,std::vector<double> &w1,std::vector<double> &w2,double t0,double tn,
	double initial_w1,double initial_w2,int N,double (*f1)(double,double,double),double (*f2)(double,double,double))
{
	double h=(tn-t0)/(double)N;
	t.clear();
	w1.clear();
	w2.clear();

	double t_temp=t0;
	double w1_temp=initial_w1;
	double w2_temp=initial_w2;
	
	t.push_back(t_temp);
	w1.push_back(w1_temp);
	w2.push_back(w2_temp);

	for(int i=1;i<=N;i++)
	{
		double a,b,a1,b1;
		a1=w1_temp;
		b1=w2_temp;

		a=a1+h*f1(t_temp,a1,b1);
		b=b1+h*f2(t_temp,a1,b1);

		w1_temp+= (h/2.0)*(f1(t_temp,a1,b1)+f1(t_temp+h,a,b));
		w2_temp+= (h/2.0)*(f2(t_temp,a1,b1)+f2(t_temp+h,a,b));
		
		t_temp=t0+(double)i*h;

		t.push_back(t_temp);
		w1.push_back(w1_temp);
		w2.push_back(w2_temp);
	}
}

int main()
{
	std::cout<<"Please choose one of the following options:"<<std::endl<<std::endl;
	std::cout<<"1.One-Dimensional ODE"<<std::endl;
	std::cout<<"	y' = cos(2t)+sin(3t) 0<=t<=6 y(0)=1"<<std::endl<<std::endl;
	std::cout<<"2.A system of ODEs"<<std::endl;
	std::cout<<"	w1' = w2"<<std::endl;
	std::cout<<"	w2' = sin(t)*e^(2t) - 2w1 + 2w2"<<std::endl;
	std::cout<<"	0<=t<=2  w1(0)=-0.4  w2(0)=-0.6"<<std::endl<<std::endl;
	std::cout<<"3.A second order ODE"<<std::endl;
	std::cout<<"	y''-2y'+y=te^t-t    0<=t<=2   y(0)=y''(0)=0"<<std::endl;
	int n,N;
	plotdata x,y,z,m;
	clear(x);
	clear(y); 
	clear(z);
	clear(m);
	std::vector<double> t;
	std::vector<double> w;
	std::vector<double> w1;
	std::vector<double> w2;
	retry:
	std::cin>>n;
	if(n!=1 && n!=2 && n!=3)
	{
		std::cout<<"Enter a correct choise  ";
		goto retry;
	}
	std::cout<<"Enter the value of N:  ";
	std::cin>>N;
	switch(n)
	{
	case(1):
		{
			//EULER
			Euler(t,w,0.0,6.0,1,N,f1prime);
			setColor(x, y, CRIMSON);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w[i];
			}
	
			//ModifiedEULER
			ModifiedEuler(t,w,0.0,6.0,1,N,f1prime);
			setColor(x, y, DARKORANGE);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w[i];
			}
	
			//RungeKutta
			RungeKutta(t,w,0.0,6.0,1,N,f1prime);
			setColor(x, y, BLUEBLUE);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w[i];
			}
			axesBotLeft (x, y, 0, 0);
			axesTopRight(x, y,  6, 2.5);
			plot(x, y,COLOR(0,160,0),"Euler-Crimson, ModifiedEuler-Orange, RungeKutta-Blue");
			break;
		}
	case(2):
		{
			//EULER
			Euler(t,w1,w2,0,2,-0.4,-0.6,N,function1,function2);
			setColor(x, y, CRIMSON);
			setColor(z, m, CRIMSON);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w1[i];
				z<<t[i];
				m<<w2[i];
			}
	
			//ModifiedEULER
			ModifiedEuler(t,w1,w2,0,2,-0.4,-0.6,N,function1,function2);
			setColor(x, y, DARKORANGE);
			setColor(z, m, DARKORANGE);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w1[i];
				z<<t[i];
				m<<w2[i];
			}
	
			//RungeKutta
			RungeKutta(t,w1,w2,0,2,-0.4,-0.6,N,function1,function2);
			setColor(x, y, BLUEBLUE);
			setColor(z, m, BLUEBLUE);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w1[i];
				z<<t[i];
				m<<w2[i];
			}
			axesBotLeft (x, y, 0, -1);
			axesTopRight(x, y,  2.1, 19);
			axesBotLeft (z, m, 0, -1);
			axesTopRight(z, m,  2.1, 55);
			plot(x, y,COLOR(0,160,0),"(t,w1) Euler-Crimson, ModifiedEuler-Orange, RungeKutta-Blue");
			plot(z, m,COLOR(0,160,0),"(t,w2) Euler-Crimson, ModifiedEuler-Orange, RungeKutta-Blue");
			break;
		}
	case(3):
		{
			//EULER
			Euler(t,w1,w2,0,2,0.0,0.0,N,function11,function22);
			setColor(x, y, CRIMSON);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w2[i];
			}
	
			//ModifiedEULER
			ModifiedEuler(t,w1,w2,0,2,0.0,0.0,N,function11,function22);
			setColor(x, y, DARKORANGE);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w2[i];
			}
	
			//RungeKutta
			RungeKutta(t,w1,w2,0,2,0.0,0.0,N,function11,function22);
			setColor(x, y, BLUEBLUE);
			for(int i=0;i<t.size();i++)
			{
				x<<t[i];
				y<<w2[i];
			}
			axesBotLeft (x, y, 0, -1);
			//axesTopRight(x, y,  2.1, 19);
			plot(x, y,COLOR(0,160,0),"Euler-Crimson, ModifiedEuler-Orange, RungeKutta-Blue");
			break;
		}
	}

	system("PAUSE");
	return 0;
}

